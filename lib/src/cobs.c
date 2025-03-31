/*
 * Copyright (c) 2025, Open Pixel Systems
 *
 * SPDX-License-Identifier: MIT
 */

#include "cobs.h"

//--------------------------------------------------
#ifdef COBS_LOG_ENABLED
#include <stdio.h>
#define ERR(...) fprintf(stderr, __VA_ARGS__)
#else
#define ERR(...)
#endif

//--------------------------------------------------
#define COBS_DELIMITER      0x00
#define COBS_MAX_ZERO_COUNT 0xFF

//--------------------------------------------------
int cobs_encode(const uint8_t *data, size_t data_len, uint8_t *encoded, size_t encoded_len)
{
	if (data == NULL || encoded == NULL) {
		ERR("[%s:%d] data == NULL || encoded == NULL\n", __func__, __LINE__);
		return -1;
	}

	if (data_len < 1) {
		ERR("[%s:%d] data_len should be at least 1\n", __func__, __LINE__);
		return -1;
	}

	uint8_t *last_zero = encoded;
	uint8_t count_since_last_zero = 1;
	size_t encoded_count = 0;

	for (size_t i = 0; i < data_len; i++) {
		// +1 for zero count and +1 for delimiter
		if ((encoded_count + 2) >= encoded_len) {
			ERR("[%s:%d] (encoded_count + 2) >= encoded_len\n", __func__, __LINE__);
			return -1;
		}

		if (data[i] == COBS_DELIMITER) {
			*last_zero = count_since_last_zero;
			last_zero = &encoded[++encoded_count];
			count_since_last_zero = 1;
		} else {
			if (count_since_last_zero == COBS_MAX_ZERO_COUNT) {
				*last_zero = COBS_MAX_ZERO_COUNT;
				last_zero = &encoded[++encoded_count];
				count_since_last_zero = 1;

				// +1 for zero count and +1 for delimiter
				if ((encoded_count + 2) >= encoded_len) {
					ERR("[%s:%d] (encoded_count + 2) >= encoded_len\n",
					    __func__, __LINE__);
					return -1;
				}
			}

			encoded[++encoded_count] = data[i];
			count_since_last_zero++;
		}
	}

	*last_zero = count_since_last_zero;
	encoded[++encoded_count] = COBS_DELIMITER;

	return (int)(encoded_count + 1);
}

//--------------------------------------------------
int cobs_decode(const uint8_t *data, size_t data_len, uint8_t *decoded, size_t decoded_len)
{
	if (data == NULL || decoded == NULL) {
		ERR("[%s:%d] data == NULL || decoded == NULL\n", __func__, __LINE__);
		return -1;
	}

	if (data_len < 3) {
		ERR("[%s:%d] data_len should be at least 3\n", __func__, __LINE__);
		return -1;
	}

	uint8_t next_zero_count = data[0];
	uint8_t count_since_last_zero = 0;
	size_t decoded_count = 0;

	for (size_t i = 1; i < data_len - 1; i++) { // Skip first zero count and delimiter
		count_since_last_zero++;

		if (data[i] == COBS_DELIMITER) {
			break;
		}

		if (decoded_count >= decoded_len) {
			ERR("[%s:%d] decoded_count >= decoded_len\n", __func__, __LINE__);
			return -1;
		}

		if (count_since_last_zero == next_zero_count) {
			if (next_zero_count != COBS_MAX_ZERO_COUNT) {
				decoded[decoded_count++] = COBS_DELIMITER;
			}

			next_zero_count = data[i];
			count_since_last_zero = 0;
		} else {
			decoded[decoded_count++] = data[i];
		}
	}

	return (int)(decoded_count);
}