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

enum [[clang::enum_extensibility(closed)]] kml_kernel_arch_amd64_idt_gate_type {
	KML_KERNEL_ARCH_AMD64_IDT_GATE_TYPE_INTERRUPT = 0xE,
	KML_KERNEL_ARCH_AMD64_IDT_GATE_TYPE_TRAP = 0xF
};

struct [[gnu::packed]] kml_kernel_arch_amd64_idt_entry {
	kml_base_u16_t offset_low;
	union kml_kernel_arch_amd64_segment_selector selector;
	kml_base_u8_t stack_table_entry : 3;
	kml_base_byte_t reserved0 : 5;
	enum kml_kernel_arch_amd64_idt_gate_type gate_type : 4;
	kml_base_byte_t reserved1 : 1;
	enum kml_kernel_arch_amd64_privilege privilege : 2;
	enum kml_base_bool present : 1;
	kml_base_u64_t offset_high : 48;
	kml_base_u32_t reserved2;
};

struct [[gnu::packed]] kml_kernel_arch_amd64_idt_descriptor {
	kml_base_u16_t size;
	struct kml_kernel_arch_amd64_idt_entry* offset;
};

enum [[clang::enum_extensibility(open)]] kml_kernel_arch_amd64_idt_vector : kml_base_u64_t {
	KML_KERNEL_ARCH_AMD64_IDT_VECTOR_EXCEPTION_DIVIDE = 0x0,
	KML_KERNEL_ARCH_AMD64_IDT_VECTOR_EXCEPTION_DEBUG = 0x1,
	KML_KERNEL_ARCH_AMD64_IDT_VECTOR_EXCEPTION_NMI = 0x2,
	KML_KERNEL_ARCH_AMD64_IDT_VECTOR_EXCEPTION_BREAKPOINT = 0x3,
	KML_KERNEL_ARCH_AMD64_IDT_VECTOR_EXCEPTION_OVERFLOW = 0x4,
	KML_KERNEL_ARCH_AMD64_IDT_VECTOR_EXCEPTION_BOUND_RANGE = 0x5,
	KML_KERNEL_ARCH_AMD64_IDT_VECTOR_EXCEPTION_INVALID_OPCODE = 0x6,
	KML_KERNEL_ARCH_AMD64_IDT_VECTOR_EXCEPTION_DEVICE_NOT_AVAILABLE = 0x7,
	KML_KERNEL_ARCH_AMD64_IDT_VECTOR_EXCEPTION_DOUBLE_FAULT = 0x8,
	KML_KERNEL_ARCH_AMD64_IDT_VECTOR_EXCEPTION_COPROCESSOR_SEGMENT_OVERRUN = 0x9,
	KML_KERNEL_ARCH_AMD64_IDT_VECTOR_EXCEPTION_INVALID_TSS = 0xA,
	KML_KERNEL_ARCH_AMD64_IDT_VECTOR_EXCEPTION_SEGMENT_NOT_PRESENT = 0xB,
	KML_KERNEL_ARCH_AMD64_IDT_VECTOR_EXCEPTION_STACK_SEGMENT_FAULT = 0xC,
	KML_KERNEL_ARCH_AMD64_IDT_VECTOR_EXCEPTION_GENERAL_PROTECTION_FAULT = 0xD,
	KML_KERNEL_ARCH_AMD64_IDT_VECTOR_EXCEPTION_PAGE_FAULT = 0xE,
	// 0xF reserved.
	KML_KERNEL_ARCH_AMD64_IDT_VECTOR_EXCEPTION_x87_FPU_FAULT = 0x10,
	KML_KERNEL_ARCH_AMD64_IDT_VECTOR_EXCEPTION_ALIGNMENT_CHECK = 0x11,
	KML_KERNEL_ARCH_AMD64_IDT_VECTOR_EXCEPTION_MACHINE_CHECK = 0x12,
	KML_KERNEL_ARCH_AMD64_IDT_VECTOR_EXCEPTION_SIMD = 0x13,
	KML_KERNEL_ARCH_AMD64_IDT_VECTOR_EXCEPTION_VIRTUALIZATION = 0x14,
	KML_KERNEL_ARCH_AMD64_IDT_VECTOR_EXCEPTION_CONTROL_PROTECTION = 0x15,
	// 0x15-0x1F reserved.
	KML_KERNEL_ARCH_AMD64_IDT_VECTOR_EXCEPTION_LAST = 0x1F,
	// 0x20-0xFF external vectors.
	KML_KERNEL_ARCH_AMD64_IDT_VECTOR_LAST = 0xFF
};

struct [[gnu::packed]] kml_kernel_arch_amd64_page_table_entry {
	enum kml_base_bool present : 1;
	enum kml_base_bool writeable : 1;
	enum kml_base_bool user : 1;
	enum kml_base_bool write_through : 1;
	enum kml_base_bool disable_cache : 1;
	enum kml_base_bool accessed : 1;
	enum kml_base_bool dirty : 1; // Free if directory.
	enum kml_base_bool large_pat : 1; // 1GiB page @ PML3, 2MiB page @ PML2, use PAT @ PML1.
	enum kml_base_bool global : 1; // Free if directory
	kml_base_u8_t free0 : 3;
	kml_base_pointer_t address : 40;
	kml_base_u16_t free1 : 7;
	kml_base_u16_t protection_key : 4; // Free if directory.
	enum kml_base_bool disable_execute : 1;
};

enum kml_base_result kml_kernel_arch_amd64_gdt_load(void);

enum kml_base_result kml_kernel_arch_amd64_idt_load(void);
