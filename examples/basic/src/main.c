/*
 * Copyright (c) 2025, Open Pixel Systems
 *
 * SPDX-License-Identifier: MIT
 */

#include <cobs.h>

#include <stdio.h>
#include <string.h>

//--------------------------------------------------
static void print_data(const uint8_t *data, size_t len)
{
	for (size_t i = 0; i < len; i++) {
		printf("0x%02X ", data[i]);
	}
	printf("\n");
}

//--------------------------------------------------
static int compare_data(const uint8_t *data1, const uint8_t *data2, size_t len)
{
	return memcmp(data1, data2, len);
}

//--------------------------------------------------
int main(void)
{
	printf("Basic example\n");

	uint8_t original[] = {0x00, 0x01, 0x00, 0x02, 0x00, 0x03, 0x00, 0x04, 0x00, 0x05};
	uint8_t encoded[sizeof(original) + 2] = {0}; // +1 for zero count and +1 for delimiter
	uint8_t decoded[sizeof(original)] = {0};

	const int encoded_len = cobs_encode(original, sizeof(original), encoded, sizeof(encoded));
	if (encoded_len < 0) {
		printf("Failed to encode buffer\n");
		return 1;
	}

	const int decoded_len = cobs_decode(encoded, encoded_len, decoded, sizeof(decoded));
	if (decoded_len < 0) {
		printf("Failed to decode buffer");
		return 1;
	}

	printf("Original data => ");
	print_data(original, sizeof(original));

	printf("Decoded data => ");
	print_data(decoded, decoded_len);

	if (compare_data(original, decoded, sizeof(original)) != 0) {
		printf("Decoded data does not match original data\n");
		return 1;
	}

	printf("Decoded data matches original data\n");
	return 0;
}