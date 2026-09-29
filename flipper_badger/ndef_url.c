#include "ndef_url.h"

#include <string.h>

// Well-known URI record: TNF=1 (Well Known), type = "U".
// Record: [0xD1][type_len=0x01][payload_len]['U'][prepend][url...]
// TLV:     [0x03][msg_len][record...][0xFE]

#define NDEF_URI_TYPE 'U'

static uint8_t ndef_uri_prepend(const char** url) {
    static const struct {
        const char* prefix;
        uint8_t code;
    } prefixes[] = {
        {"https://www.", 0x02},
        {"http://www.", 0x01},
        {"https://", 0x04},
        {"http://", 0x03},
    };

    for(size_t i = 0; i < sizeof(prefixes) / sizeof(prefixes[0]); i++) {
        size_t len = strlen(prefixes[i].prefix);
        if(strncmp(*url, prefixes[i].prefix, len) == 0) {
            *url += len;
            return prefixes[i].code;
        }
    }

    return 0x00;
}

size_t ndef_url_build(const char* url, uint8_t* out, size_t out_size) {
    if(!url || !out) return 0;

    const char* body = url;
    uint8_t prepend = ndef_uri_prepend(&body);

    size_t url_len = strlen(body);
    if(url_len > NDEF_URL_MAX_LEN) return 0;

    size_t payload_len = 1 + url_len; // prepend byte + url
    size_t record_len = 3 + 1 + payload_len; // flags + type_len + payload_len_field + payload
    size_t msg_len = record_len;
    size_t tlv_len = 2 + msg_len + 1; // TLV byte + len byte + record + terminator

    if(tlv_len > out_size) return 0;

    size_t pos = 0;
    out[pos++] = 0x03; // NDEF message TLV
    out[pos++] = (uint8_t)msg_len;

    out[pos++] = 0xD1; // MB=1, ME=1, SR=1, TNF=1 (Well Known)
    out[pos++] = 0x01; // type length
    out[pos++] = (uint8_t)payload_len;
    out[pos++] = NDEF_URI_TYPE;
    out[pos++] = prepend;
    memcpy(&out[pos], body, url_len);
    pos += url_len;

    out[pos++] = 0xFE; // terminator

    return pos;
}
