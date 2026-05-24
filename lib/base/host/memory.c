// SPDX-Licence-Identifier: X11
// Copyright (C) 2025 Emily "TTG" Banerjee <prs.ttg+kml@pm.me>

#include <kml/base/memory.h>

#include <string.h>

void kml_base_memory_fill(
		kml_base_byte_t* buffer, const kml_base_size_t size, const kml_base_byte_t value) {

	memset(buffer, value, size);
}
