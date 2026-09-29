#include "badger_nfc.h"
#include "ndef_url.h"

#include <furi.h>
#include <furi_hal.h>
#include <furi_hal_nfc.h>
#include <furi_hal_random.h>

#include <nfc/nfc.h>
#include <nfc/nfc_listener.h>
#include <nfc/protocols/mf_ultralight/mf_ultralight.h>

#define TAG "badger_nfc"

#define NXP_MANUFACTURER_ID   (0x04)
#define NTAG213_PAGES_TOTAL   (45)
#define NTAG213_USER_MEM_SIZE (144)

struct BadgerNfc {
    Nfc* nfc;
    NfcListener* listener;
    MfUltralightData* mfu;
};

BadgerNfc* badger_nfc_alloc(void) {
    BadgerNfc* instance = malloc(sizeof(BadgerNfc));

    instance->nfc = NULL;
    instance->listener = NULL;
    instance->mfu = NULL;

    return instance;
}

void badger_nfc_free(BadgerNfc* instance) {
    if(!instance) return;

    badger_nfc_stop(instance);
    free(instance);
}

static MfUltralightData* badger_nfc_build_ntag213(const char* url) {
    MfUltralightData* mfu = mf_ultralight_alloc();

    // UID: NXP manufacturer byte + random
    uint8_t uid[7];
    uid[0] = NXP_MANUFACTURER_ID;
    furi_hal_random_fill_buf(&uid[1], 6);
    uid[3] |= 0x01; // avoid forbidden 0x88
    uid[6] &= 0x0F;
    uid[6] |= 0x80;
    mf_ultralight_set_uid(mfu, uid, 7);

    mfu->iso14443_3a_data->atqa[0] = 0x44;
    mfu->iso14443_3a_data->atqa[1] = 0x00;
    mfu->iso14443_3a_data->sak = 0x00;

    mfu->type = MfUltralightTypeNTAG213;
    mfu->pages_total = NTAG213_PAGES_TOTAL;
    mfu->pages_read = NTAG213_PAGES_TOTAL;

    // GET_VERSION response (NTAG21x, storage size 0x0F)
    mfu->version.header = 0x00;
    mfu->version.vendor_id = 0x04;
    mfu->version.prod_type = 0x04;
    mfu->version.prod_subtype = 0x02;
    mfu->version.prod_ver_major = 0x01;
    mfu->version.prod_ver_minor = 0x00;
    mfu->version.storage_size = 0x0F;
    mfu->version.protocol_type = 0x03;

    mfu->page[2].data[1] = 0x48; // internal byte

    // Capability Container: E1 10 12 00 (144 B user memory)
    mfu->page[3].data[0] = 0xE1;
    mfu->page[3].data[1] = 0x10;
    mfu->page[3].data[2] = 0x12;
    mfu->page[3].data[3] = 0x00;

    // Config pages: CFG0 (41), CFG1 (42), PWD (43)
    uint16_t config_index = NTAG213_PAGES_TOTAL - 4; // 41
    mfu->page[config_index].data[0] = 0x04; // STRG_MOD_EN
    mfu->page[config_index].data[3] = 0xff; // AUTH0
    mfu->page[config_index + 1].data[1] = 0x05; // VCTID
    memset(&mfu->page[config_index + 2], 0xff, sizeof(MfUltralightPage)); // PWD

    // NDEF URL TLV into user memory (pages 4..39)
    uint8_t ndef[NDEF_URL_MAX_LEN + 8];
    size_t ndef_len = ndef_url_build(url, ndef, sizeof(ndef));
    if(ndef_len == 0 || ndef_len > NTAG213_USER_MEM_SIZE) {
        mf_ultralight_free(mfu);
        return NULL;
    }

    memcpy(&mfu->page[4].data[0], ndef, ndef_len);

    return mfu;
}

bool badger_nfc_start(BadgerNfc* instance, const char* url) {
    furi_assert(instance);

    if(furi_hal_nfc_is_hal_ready() != FuriHalNfcErrorNone) {
        FURI_LOG_E(TAG, "NFC HAL not ready");
        return false;
    }

    instance->nfc = nfc_alloc();

    instance->mfu = badger_nfc_build_ntag213(url);
    if(!instance->mfu) {
        nfc_free(instance->nfc);
        instance->nfc = NULL;
        return false;
    }

    instance->listener = nfc_listener_alloc(
        instance->nfc, NfcProtocolMfUltralight, (const NfcDeviceData*)instance->mfu);
    nfc_listener_start(instance->listener, NULL, NULL);

    return true;
}

void badger_nfc_stop(BadgerNfc* instance) {
    furi_assert(instance);

    if(instance->listener) {
        nfc_listener_stop(instance->listener);
        nfc_listener_free(instance->listener);
        instance->listener = NULL;
    }

    if(instance->mfu) {
        mf_ultralight_free(instance->mfu);
        instance->mfu = NULL;
    }

    if(instance->nfc) {
        nfc_free(instance->nfc);
        instance->nfc = NULL;
    }
}
