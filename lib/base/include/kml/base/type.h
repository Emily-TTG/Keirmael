// SPDX-Licence-Identifier: X11
// Copyright (C) 2025 Emily "TTG" Banerjee <prs.ttg+kml@pm.me>

#pragma once

#include <kml/base/environment.h>

#define KML_BASE_BYTE_BIT (8u)

typedef unsigned char kml_base_byte_t;

enum [[clang::enum_extensibility(closed)]] kml_base_bool {
	KML_BASE_BOOL_FALSE = 0,
	KML_BASE_BOOL_TRUE = 1
};

typedef unsigned char kml_base_u8_t;
typedef unsigned short kml_base_u16_t;
typedef unsigned int kml_base_u32_t;
typedef unsigned long long kml_base_u64_t;

typedef signed char kml_base_s8_t;
typedef short kml_base_s16_t;
typedef int kml_base_s32_t;
typedef long long kml_base_s64_t;

#ifdef KML_BASE_ARCH_64
typedef kml_base_u64_t kml_base_pointer_t;
typedef kml_base_u64_t kml_base_size_t;
#endif
