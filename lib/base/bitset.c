// SPDX-Licence-Identifier: X11
// Copyright (C) 2025 Emily "TTG" Banerjee <prs.ttg+kml@pm.me>

#include <kml/base/bitset.h>
#include <kml/base/type.h>

void kml_base_bitset_set(kml_base_byte_t* buffer, kml_base_size_t bit, enum kml_base_bool value) {
	if(value) buffer[bit / KML_BASE_BYTE_BIT] |= 1 << (bit % KML_BASE_BYTE_BIT);
	else buffer[bit / KML_BASE_BYTE_BIT] &= ~(1 << (bit % KML_BASE_BYTE_BIT));
}

enum kml_base_bool kml_base_bitset_get(kml_base_byte_t* buffer, kml_base_size_t bit) {
	return !!(buffer[bit / KML_BASE_BYTE_BIT] & (1 << (bit % KML_BASE_BYTE_BIT)));
}
