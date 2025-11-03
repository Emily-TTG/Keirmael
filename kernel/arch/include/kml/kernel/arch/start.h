// SPDX-Licence-Identifier: X11
// Copyright (C) 2025 Emily "TTG" Banerjee <prs.ttg+kml@pm.me>

#pragma once

struct kml_base_allocator_region;

[[noreturn]]
void kml_kernel_arch_start(void*);

enum kml_base_result kml_kernel_arch_process_boot_data(void*, struct kml_base_allocator_region**);
