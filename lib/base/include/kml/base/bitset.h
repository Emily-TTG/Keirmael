// SPDX-Licence-Identifier: X11
// Copyright (C) 2025 Emily "TTG" Banerjee <prs.ttg+kml@pm.me>

#pragma once

#include <kml/base/result.h>
#include <kml/base/type.h>

/**
 * Set the specified bit of the input buffer.
 * \param[in,out] 0 The buffer to treat as a bitset.
 * \param[in] 1 The index of the bit to set.
 * \param[in] 2 The value to set the bit to.
 */
void kml_base_bitset_set(kml_base_byte_t*, kml_base_size_t, enum kml_base_bool);

/**
 * Get the specified bit of the input buffer.
 * \param[in,out] 0 The buffer to treat as a bitset.
 * \param[in] 1 The index of the bit to get.
 * \return The value of the bit.
 */
[[nodiscard]]
enum kml_base_bool kml_base_bitset_get(kml_base_byte_t*, kml_base_size_t);
