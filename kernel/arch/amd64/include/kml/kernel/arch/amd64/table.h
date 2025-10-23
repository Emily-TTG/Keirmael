// SPDX-Licence-Identifier: X11
// Copyright (C) 2025 Emily "TTG" Banerjee <prs.ttg+kml@pm.me>

#pragma once

#include <kml/base/type.h>
#include <kml/base/result.h>

#include <kml/kernel/arch/amd64/cpu.h>

enum [[clang::enum_extensibility(closed)]] kml_kernel_arch_amd64_gdt_entry_accessibility {
	KML_KERNEL_ARCH_AMD64_GDT_ENTRY_ACCESSIBILITY_CODE_UNREADABLE = 0,
	KML_KERNEL_ARCH_AMD64_GDT_ENTRY_ACCESSIBILITY_CODE_READABLE = 1,

	KML_KERNEL_ARCH_AMD64_GDT_ENTRY_ACCESSIBILITY_DATA_UNWRITEABLE = 0,
	KML_KERNEL_ARCH_AMD64_GDT_ENTRY_ACCESSIBILITY_DATA_WRITEABLE = 1
};

enum [[clang::enum_extensibility(closed)]] kml_kernel_arch_amd64_gdt_entry_direction_conforming {
	KML_KERNEL_ARCH_AMD64_GDT_ENTRY_CODE_MATCH = 0,
	KML_KERNEL_ARCH_AMD64_GDT_ENTRY_CODE_CONFORMING = 1,

	KML_KERNEL_ARCH_AMD64_GDT_ENTRY_DATA_DIRECTION_UP = 0,
	KML_KERNEL_ARCH_AMD64_GDT_ENTRY_DATA_DIRECTION_DOWN = 1
};

enum [[clang::enum_extensibility(closed)]] kml_kernel_arch_amd64_gdt_entry_type {
	KML_KERNEL_ARCH_AMD64_GDT_ENTRY_TYPE_SYSTEM = 0,
	KML_KERNEL_ARCH_AMD64_GDT_ENTRY_TYPE_CODE_DATA = 1
};

enum [[clang::enum_extensibility(closed)]] kml_kernel_arch_amd64_gdt_entry_size {
	KML_KERNEL_ARCH_AMD64_GDT_ENTRY_SIZE_16 = 0,
	KML_KERNEL_ARCH_AMD64_GDT_ENTRY_SIZE_32 = 1,

	KML_KERNEL_ARCH_AMD64_GDT_ENTRY_SIZE_64 = KML_KERNEL_ARCH_AMD64_GDT_ENTRY_SIZE_16,
};

enum [[clang::enum_extensibility(closed)]] kml_kernel_arch_amd64_gdt_entry_granularity {
	KML_KERNEL_ARCH_AMD64_GDT_ENTRY_GRANULARITY_BYTE = 0,
	KML_KERNEL_ARCH_AMD64_GDT_ENTRY_GRANULARITY_PAGE = 1
};

enum [[clang::enum_extensibility(closed)]] kml_kernel_arch_amd64_gdt_entry_system_type {
	KML_KERNEL_ARCH_AMD64_GDT_ENTRY_SYSTEM_TYPE_LDT = 0x2,
    KML_KERNEL_ARCH_AMD64_GDT_ENTRY_SYSTEM_TYPE_TSS_AVAILABLE = 0x9,
    KML_KERNEL_ARCH_AMD64_GDT_ENTRY_SYSTEM_TYPE_TSS_BUSY = 0xB
};

struct [[gnu::packed]] kml_kernel_arch_amd64_gdt_entry {
	kml_base_u16_t limit_low;
	kml_base_u32_t base_low : 24;
	union [[gnu::packed]] {
		struct [[gnu::packed]] {
			enum kml_base_bool accessed : 1;
			enum kml_kernel_arch_amd64_gdt_entry_accessibility accessibility : 1;
			enum kml_kernel_arch_amd64_gdt_entry_direction_conforming direction_conforming : 1;
			enum kml_base_bool executable : 1;
			enum kml_kernel_arch_amd64_gdt_entry_type type : 1;
			enum kml_kernel_arch_amd64_privilege privilege : 2;
			enum kml_base_bool present : 1;
		} code_data;
		struct [[gnu::packed]] {
			enum kml_kernel_arch_amd64_gdt_entry_system_type system_type : 4;
			enum kml_kernel_arch_amd64_gdt_entry_type type : 1;
			enum kml_kernel_arch_amd64_privilege privilege : 2;
			enum kml_base_bool present : 1;
		} system;
	} access;
	kml_base_u8_t limit_high : 4;
	kml_base_byte_t reserved0 : 1;
	enum kml_base_bool long_mode : 1;
	enum kml_kernel_arch_amd64_gdt_entry_size size : 1;
	enum kml_kernel_arch_amd64_gdt_entry_granularity granularity : 1;
	kml_base_u8_t base_high;
/*
	kml_base_u64_t base_high : 40;
	kml_base_u32_t reserved1;
*/
};

struct [[gnu::packed]] kml_kernel_arch_amd64_gdt_descriptor {
	kml_base_u16_t size;
	struct kml_kernel_arch_amd64_gdt_entry* offset;
};

enum kml_base_result kml_kernel_arch_amd64_gdt_load(void);
