/*
 * Copyright (c) 2025, Open Pixel Systems
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include <stddef.h>
#include <stdint.h>

/**
 * @brief Encode data using COBS algorithm
 * @param data Pointer to data to encode
 * @param data_len Length of data to encode
 * @param encoded Pointer to buffer to store encoded data
 * @return Number of bytes in the encoded buffer on success, -1 on error
 */
int cobs_encode(const uint8_t *data, size_t data_len, uint8_t *encoded, size_t encoded_len);

/**
 * @brief Decode data using COBS algorithm
 * @param data Pointer to data to decode
 * @param data_len Length of data to decode
 * @param decoded Pointer to buffer to store decoded data
 * @param decoded_len Length of decoded buffer
 * @return Number of bytes in the decoded buffer on success, -1 on error
 */
int cobs_decode(const uint8_t *data, size_t data_len, uint8_t *decoded, size_t decoded_len);