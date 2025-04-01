/*
 * Copyright (c) 2025, Open Pixel Systems
 *
 * SPDX-License-Identifier: MIT
 */

#include <cobs.h>

#include <stdio.h>
#include <string.h>

/**
 * @brief Print data in hex format
 * @param data Pointer to data
 * @param len Length of data
 */
//--------------------------------------------------
static void print_data(const uint8_t *data, size_t len)
{
	for (size_t i = 0; i < len; i++) {
		printf("0x%02X ", data[i]);
	}
	printf("\n");
}

/**
 * @brief Compare two data buffers
 * @param data1 Pointer to first data buffer
 * @param data2 Pointer to second data buffer
 * @param len Length of data buffers (must be equal)
 */
//--------------------------------------------------
static int compare_data(const uint8_t *data1, const uint8_t *data2, size_t len)
{
	return memcmp(data1, data2, len);
}

//--------------------------------------------------
int main(void)
{
	printf("Basic example\n");

	// Define buffers
	uint8_t original[] = {0x00, 0x01, 0x00, 0x02, 0x00, 0x03, 0x00, 0x04, 0x00, 0x05};
	uint8_t encoded[sizeof(original) + 2] = {0}; // +1 for zero count and +1 for delimiter
	uint8_t decoded[sizeof(original)] = {0};

	// Encode the original data using COBS algorithm
	const int encoded_len = cobs_encode(original, sizeof(original), encoded, sizeof(encoded));
	if (encoded_len < 0) {
		printf("Failed to encode buffer\n");
		return 1;
	}

	// Decode the encoded data using COBS algorithm
	const int decoded_len = cobs_decode(encoded, encoded_len, decoded, sizeof(decoded));
	if (decoded_len < 0) {
		printf("Failed to decode buffer");
		return 1;
	}

	// Print the original data
	printf("Original data => ");
	print_data(original, sizeof(original));

	// Print the encoded data
	printf("Encoded data  => ");
	print_data(encoded, encoded_len);

	// Print the encoded data
	printf("Decoded data  => ");
	print_data(decoded, decoded_len);

	// Print the decoded data
	if (compare_data(original, decoded, sizeof(original)) != 0) {
		// Print error message
		printf("Decoded data does not match original data\n");
		return 1;
	}

	// Print success message
	printf("Decoded data matches original data\n");
	return 0;
}