// SPDX-Licence-Identifier: X11
// Copyright (C) 2025 Emily "TTG" Banerjee <prs.ttg+kml@pm.me>

#pragma once

#include <kml/base/result.h>

#include <kml/kernel/memory.h>

struct kml_base_allocator_region;

typedef void* kml_kernel_arch_boot_data_t;

[[noreturn]]
void kml_kernel_arch_start(void*);

kml_base_pointer_t kml_kernel_arch_allocated_physical(kml_base_byte_t*);
kml_base_byte_t* kml_kernel_arch_physical_allocated(kml_base_pointer_t);

enum kml_base_result kml_kernel_arch_boot_populate_allocator(
		kml_kernel_arch_boot_data_t, struct kml_base_allocator_region**);

enum kml_base_result kml_kernel_arch_boot_map_default(
		kml_kernel_arch_boot_data_t, struct kml_base_allocator_region*, kml_kernel_memory_mapping_context_t);
