// SPDX-Licence-Identifier: X11
// Copyright (C) 2025 Emily "TTG" Banerjee <prs.ttg+kml@pm.me>

#include <kml/base/environment.h>

#include <kml/kernel/arch/amd64/table.h>

enum [[clang::enum_extensibility(closed)]] kml_kernel_arch_amd64_gdt_index {
	KML_KERNEL_ARCH_AMD64_GDT_INDEX_NULL,
	KML_KERNEL_ARCH_AMD64_GDT_INDEX_KERNEL_CODE,
	KML_KERNEL_ARCH_AMD64_GDT_INDEX_KERNEL_DATA,

	KML_KERNEL_ARCH_AMD64_GDT_INDEX_MAX
};

static struct kml_kernel_arch_amd64_gdt_entry kml_global_kernel_arch_amd64_gdt[KML_KERNEL_ARCH_AMD64_GDT_INDEX_MAX] = {
	// Null Descriptor.
	[KML_KERNEL_ARCH_AMD64_GDT_INDEX_NULL] = {},

	// Kernel Code.
	[KML_KERNEL_ARCH_AMD64_GDT_INDEX_KERNEL_CODE] = {
		.base_low = 0, .base_high = 0,
		.limit_low = 0xFFFF, .limit_high = 0xF,
		.access = {
			.code_data =  {
				.accessed = KML_BASE_BOOL_TRUE,
				.accessibility = KML_KERNEL_ARCH_AMD64_GDT_ENTRY_ACCESSIBILITY_CODE_READABLE,
				.direction_conforming = KML_KERNEL_ARCH_AMD64_GDT_ENTRY_CODE_MATCH,
				.executable = KML_BASE_BOOL_TRUE,
				.type = KML_KERNEL_ARCH_AMD64_GDT_ENTRY_TYPE_CODE_DATA,
				.privilege = KML_KERNEL_ARCH_AMD64_CPU_PRIVILEGE_RING0,
				.present = KML_BASE_BOOL_TRUE
			}
		},
		.long_mode = KML_BASE_BOOL_TRUE,
		.size = KML_KERNEL_ARCH_AMD64_GDT_ENTRY_SIZE_64,
		.granularity = KML_KERNEL_ARCH_AMD64_GDT_ENTRY_GRANULARITY_PAGE
	},

	// Kernel Data.
	[KML_KERNEL_ARCH_AMD64_GDT_INDEX_KERNEL_DATA] = {
		.base_low = 0, .base_high = 0,
		.limit_low = 0xFFFF, .limit_high = 0xF,
		.access = {
			.code_data =  {
				.accessed = KML_BASE_BOOL_TRUE,
				.accessibility = KML_KERNEL_ARCH_AMD64_GDT_ENTRY_ACCESSIBILITY_DATA_WRITEABLE,
				.direction_conforming = KML_KERNEL_ARCH_AMD64_GDT_ENTRY_DATA_DIRECTION_UP,
				.executable = KML_BASE_BOOL_FALSE,
				.type = KML_KERNEL_ARCH_AMD64_GDT_ENTRY_TYPE_CODE_DATA,
				.privilege = KML_KERNEL_ARCH_AMD64_CPU_PRIVILEGE_RING0,
				.present = KML_BASE_BOOL_TRUE
			}
		},
		.long_mode = KML_BASE_BOOL_TRUE,
		.size = KML_KERNEL_ARCH_AMD64_GDT_ENTRY_SIZE_64,
		.granularity = KML_KERNEL_ARCH_AMD64_GDT_ENTRY_GRANULARITY_PAGE
	}

	// TODO: User Code/Data + TSS.
};

static struct kml_kernel_arch_amd64_gdt_descriptor kml_global_kernel_arch_amd64_gdt_descriptor = {
	sizeof(kml_global_kernel_arch_amd64_gdt) - 1,
	kml_global_kernel_arch_amd64_gdt
};

static struct kml_kernel_arch_amd64_idt_entry kml_global_kernel_arch_amd64_idt[KML_KERNEL_ARCH_AMD64_IDT_VECTOR_LAST] = {};

static struct kml_kernel_arch_amd64_idt_descriptor kml_global_kernel_arch_amd64_idt_descriptor = {
	sizeof(kml_global_kernel_arch_amd64_idt) - 1,
	kml_global_kernel_arch_amd64_idt
};

static const union kml_kernel_arch_amd64_segment_selector kml_global_kernel_arch_amd64_segment_code_selector = {
	.privilege = KML_KERNEL_ARCH_AMD64_CPU_PRIVILEGE_RING0,
	.type = KML_KERNEL_ARCH_AMD64_SEGMENT_SELECTOR_TYPE_GDT,
	.index = KML_KERNEL_ARCH_AMD64_GDT_INDEX_KERNEL_CODE
};

static const union kml_kernel_arch_amd64_segment_selector kml_global_kernel_arch_amd64_segment_data_selector = {
	.privilege = KML_KERNEL_ARCH_AMD64_CPU_PRIVILEGE_RING0,
	.type = KML_KERNEL_ARCH_AMD64_SEGMENT_SELECTOR_TYPE_GDT,
	.index = KML_KERNEL_ARCH_AMD64_GDT_INDEX_KERNEL_DATA
};

enum kml_base_result kml_kernel_arch_amd64_gdt_load(void) {
	KML_BASE_ASM("lgdt %[gdtr]" :: [gdtr]"p"(&kml_global_kernel_arch_amd64_gdt_descriptor));

	KML_BASE_ASM(
		"movw %[data_selector], %%ax\n"
		"movw %%ax, %%ds\n"
		"movw %%ax, %%es\n"
		"movw %%ax, %%fs\n"
		"movw %%ax, %%gs\n"
		"movw %%ax, %%ss\n"

		"pushq %[code_selector]\n"
		"leaq .kml_kernel_arch_amd64_gdt_reload_cs(%%rip), %%rax\n"
		"pushq %%rax\n"
		"lretq\n"
		".kml_kernel_arch_amd64_gdt_reload_cs:\n" ::
		[code_selector]"r"((kml_base_u64_t) kml_global_kernel_arch_amd64_segment_code_selector.raw),
		[data_selector]"r"(kml_global_kernel_arch_amd64_segment_data_selector.raw) : "rax");

	return KML_BASE_RESULT_OK;
}

extern void kml_kernel_arch_amd64_interrupt_handler_0x0(void);
extern void kml_kernel_arch_amd64_interrupt_handler_0xFF(void);

enum kml_base_result kml_kernel_arch_amd64_idt_load(void) {
	// TODO: We can just codegen a prefilled IDT with this logic and a special ISR section at a fixed address
	//		 instead of this unwieldy nonsense.
	for(kml_base_size_t i = 0; i < KML_KERNEL_ARCH_AMD64_IDT_VECTOR_LAST; ++i) {
		enum kml_kernel_arch_amd64_idt_gate_type type;

		if(i <= KML_KERNEL_ARCH_AMD64_IDT_VECTOR_EXCEPTION_LAST) {
			type = KML_KERNEL_ARCH_AMD64_IDT_GATE_TYPE_TRAP;
		}
		else type = KML_KERNEL_ARCH_AMD64_IDT_GATE_TYPE_INTERRUPT;

		kml_base_pointer_t offset =
				(kml_base_pointer_t) kml_kernel_arch_amd64_interrupt_handler_0x0 +
				i * KML_KERNEL_ARCH_AMD64_INTERRUPT_HANDLER_STRIDE;

		kml_global_kernel_arch_amd64_idt[i] = (struct kml_kernel_arch_amd64_idt_entry) {
			.offset_low = offset & 0xFFFF, .offset_high = offset >> 16,
			.selector = kml_global_kernel_arch_amd64_segment_code_selector,
			// TODO: Populate.
			.stack_table_entry = 0,
			.gate_type = type,
			.privilege = KML_KERNEL_ARCH_AMD64_CPU_PRIVILEGE_RING0,
			.present = KML_BASE_BOOL_TRUE
		};
	}

	KML_BASE_ASM("lidt %[idtr]" :: [idtr]"p"(&kml_global_kernel_arch_amd64_idt_descriptor));

	return KML_BASE_RESULT_OK;
}
