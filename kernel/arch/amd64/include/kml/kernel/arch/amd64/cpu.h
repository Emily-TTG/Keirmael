// SPDX-Licence-Identifier: X11
// Copyright (C) 2025 Emily "TTG" Banerjee <prs.ttg+kml@pm.me>

#pragma once

#include <kml/base/type.h>

enum [[clang::enum_extensibility(closed)]] kml_kernel_arch_amd64_privilege {
	KML_KERNEL_ARCH_AMD64_CPU_PRIVILEGE_RING0 = 0,
	KML_KERNEL_ARCH_AMD64_CPU_PRIVILEGE_RING1 = 1,
	KML_KERNEL_ARCH_AMD64_CPU_PRIVILEGE_RING2 = 2,
	KML_KERNEL_ARCH_AMD64_CPU_PRIVILEGE_RING3 = 3
};

enum [[clang::enum_extensibility(closed)]] kml_kernel_arch_amd64_segment_selector_type {
	KML_KERNEL_ARCH_AMD64_SEGMENT_SELECTOR_TYPE_GDT = 0,
	KML_KERNEL_ARCH_AMD64_SEGMENT_SELECTOR_TYPE_LDT = 1
};

union [[gnu::packed]] kml_kernel_arch_amd64_segment_selector {
	struct [[gnu::packed]] {
		enum kml_kernel_arch_amd64_privilege privilege : 2;
		enum kml_kernel_arch_amd64_segment_selector_type type : 1;
		kml_base_u16_t index : 13;
	};
	kml_base_u16_t raw;
};
