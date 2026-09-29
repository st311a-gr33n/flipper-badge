#pragma once

#include <stddef.h>
#include <stdint.h>

// NTAG213 user memory is 144 bytes; this leaves headroom for the TLV wrapper.
#define NDEF_URL_MAX_LEN 136

size_t ndef_url_build(const char* url, uint8_t* out, size_t out_size);
