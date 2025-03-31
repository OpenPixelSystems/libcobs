/*
 * Copyright (c) 2025, Open Pixel Systems
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include <stddef.h>
#include <stdint.h>

int cobs_encode(const uint8_t *data, size_t data_len, uint8_t *encoded, size_t dst_len);
int cobs_decode(const uint8_t *data, size_t data_len, uint8_t *decoded, size_t dst_len);