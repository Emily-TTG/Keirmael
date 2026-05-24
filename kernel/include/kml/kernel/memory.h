// SPDX-Licence-Identifier: X11
// Copyright (C) 2025 Emily "TTG" Banerjee <prs.ttg+kml@pm.me>

#pragma once

#include <kml/base/result.h>
#include <kml/base/type.h>

struct kml_base_allocator_region;

// NOTE: Values equal to the respective AMD64 PML for a page of that size - 1.
enum [[clang::enum_extensibility(closed)]] kml_kernel_memory_mapping_granularity {
	KML_KERNEL_MEMORY_MAPPING_GRANULARITY_4KIB = 0,
	KML_KERNEL_MEMORY_MAPPING_GRANULARITY_2MIB = 1,
	KML_KERNEL_MEMORY_MAPPING_GRANULARITY_1GIB = 2
};

enum [[clang::enum_extensibility(flag)]] kml_kernel_memory_mapping_protection {
	KML_KERNEL_MEMORY_MAPPING_PROTECTION_READ = 0,
	KML_KERNEL_MEMORY_MAPPING_PROTECTION_WRITE = 1,
	KML_KERNEL_MEMORY_MAPPING_PROTECTION_EXECUTE = 1 << 1,
};

struct kml_kernel_memory_mapping {
	kml_base_byte_t* mapped;
	kml_base_size_t count;
	enum kml_kernel_memory_mapping_granularity granularity;
};

typedef void* kml_kernel_memory_mapping_context_t;

enum kml_base_result kml_kernel_memory_mapping_context_new(
		struct kml_base_allocator_region*, kml_kernel_memory_mapping_context_t*);

enum kml_base_result kml_kernel_memory_mapping_context_load(kml_kernel_memory_mapping_context_t);

enum kml_base_result kml_kernel_memory_mapping_new(
		struct kml_base_allocator_region*, kml_kernel_memory_mapping_context_t,
		struct kml_kernel_memory_mapping*, kml_base_pointer_t, kml_base_pointer_t, kml_base_size_t,
		enum kml_kernel_memory_mapping_granularity, enum kml_kernel_memory_mapping_protection);

enum kml_base_result kml_kernel_memory_mapping_delete(struct kml_kernel_memory_mapping*);
