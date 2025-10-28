/*
 * Copyright (c) 2025, Open Pixel Systems
 *
 * SPDX-License-Identifier: MIT
 */

#include "cobs.h"

// Define COBS_LOG_ENABLED to enable logging
//--------------------------------------------------
#ifdef COBS_LOG_ENABLED
#include <stdio.h>
#define ERR(...) fprintf(stderr, __VA_ARGS__)
#else
#define ERR(...)
#endif

// Defines used in COBS encoding/decoding
//--------------------------------------------------
#define COBS_DELIMITER      0x00
#define COBS_MAX_ZERO_COUNT 0xFF

//--------------------------------------------------
int cobs_encode(const uint8_t *data, size_t data_len, uint8_t *encoded, size_t encoded_len)
{
	// Check for NULL pointers
	if (data == NULL || encoded == NULL) {
		// Encoding failed, return -1
		ERR("[%s:%d] data == NULL || encoded == NULL\n", __func__, __LINE__);
		return -1;
	}

	// Check for minimum data length
	if (data_len < 1) {
		// Encoding failed, return -1
		ERR("[%s:%d] data_len should be at least 1\n", __func__, __LINE__);
		return -1;
	}

	uint8_t *last_zero = encoded;
	uint8_t count_since_last_zero = 1;
	size_t encoded_count = 0;

	for (size_t i = 0; i < data_len; i++) {

		// Check for maximum zero count
        if (count_since_last_zero == COBS_MAX_ZERO_COUNT) {
            // Set the maximum zero count in the last zero byte, save the
            // current byte as the last zero byte and reset the count since last
            // zero
            *last_zero = COBS_MAX_ZERO_COUNT;
            last_zero = &encoded[++encoded_count];
            count_since_last_zero = 1;

            // Check for buffer overflow, +1 for zero count and +1 for delimiter
			if ((encoded_count + 2) >= encoded_len) {
				// Encoding failed, return -1
				ERR("[%s:%d] (encoded_count + 2) >= encoded_len\n",
				    __func__, __LINE__);
				return -1;
			}
        }

		// Check for buffer overflow, +1 for zero count and +1 for delimiter
		if ((encoded_count + 2) >= encoded_len) {
			// Encoding failed, return -1
			ERR("[%s:%d] (encoded_count + 2) >= encoded_len\n", __func__, __LINE__);
			return -1;
		}

		// Check for zero byte
		if (data[i] == COBS_DELIMITER) {
			// Set the count since last zero in the last zero byte, save the current
			// byte as the last zero byte and reset the count since last zero
			*last_zero = count_since_last_zero;
			last_zero = &encoded[++encoded_count];
			count_since_last_zero = 1;
		} else {

			// Normal byte, copy the data byte to the encoded buffer
			encoded[++encoded_count] = data[i];

			// Increment count since last zero
			count_since_last_zero++;
		}
	}

	// Set the count since last zero in the last zero byte
	*last_zero = count_since_last_zero;

	// Set the delimiter at the end of the encoded buffer
	encoded[++encoded_count] = COBS_DELIMITER;

	// Return the number of bytes in the encoded buffer
	return (int)(encoded_count + 1);
}

//--------------------------------------------------
int cobs_decode(const uint8_t *data, size_t data_len, uint8_t *decoded, size_t decoded_len)
{
	// Check for NULL pointers
	if (data == NULL || decoded == NULL) {
		// Decoding failed, return -1
		ERR("[%s:%d] data == NULL || decoded == NULL\n", __func__, __LINE__);
		return -1;
	}

	// Check for minimum data length, 1 zero count, at least 1 data byte and 1 delimiter
	if (data_len < 3) {
		// Decoding failed, return -1
		ERR("[%s:%d] data_len should be at least 3\n", __func__, __LINE__);
		return -1;
	}

	uint8_t next_zero_count = data[0];
	uint8_t count_since_last_zero = 0;
	size_t decoded_count = 0;

	// Loop through the data buffer
	for (size_t i = 1; i < data_len - 1; i++) { // Skip first zero count and delimiter
		// Increment zero count
		count_since_last_zero++;

		// Check for zero byte, zero byte indicates the end of the encoded data
		if (data[i] == COBS_DELIMITER) {
			// Stop decoding if the zero byte is detected before the next zero count
			break;
		}

		// Check for buffer overflow
		if (decoded_count >= decoded_len) {
			// Decoding failed, return -1
			ERR("[%s:%d] decoded_count >= decoded_len\n", __func__, __LINE__);
			return -1;
		}

		// Check if last zero count is reached
		if (count_since_last_zero == next_zero_count) {
			// Check if the next zero count is not the maximum zero count
			if (next_zero_count != COBS_MAX_ZERO_COUNT) {
				// Copy the zero byte to the decoded buffer (current byte was
				// originally a zero byte)
				decoded[decoded_count++] = COBS_DELIMITER;
			}

			// Set the next zero count to the current byte
			next_zero_count = data[i];

			// Reset count since last zero
			count_since_last_zero = 0;
		} else {
			// Normal byte, copy the data byte to the decoded buffer
			decoded[decoded_count++] = data[i];
		}
	}

	// Return the number of bytes in the decoded buffer
	return (int)(decoded_count);
}