// SPDX-Licence-Identifier: X11
// Copyright (C) 2025 Emily "TTG" Banerjee <prs.ttg+kml@pm.me>

#include <kml/base/log.h>
#include <kml/base/type.h>

#include <kml/kernel/arch/amd64/table.h>

struct [[gnu::packed]] kml_kernel_arch_amd64_interrupt_service_routine_data {
	kml_base_u64_t error_code;
	kml_base_u64_t rip;
	kml_base_u64_t cs;
	kml_base_u64_t rflags;
	kml_base_u64_t rsp;
	kml_base_u64_t ss;
};

void kml_kernel_arch_amd64_interrupt_service_routine(
		const enum kml_kernel_arch_amd64_idt_vector vector,
		struct kml_kernel_arch_amd64_interrupt_service_routine_data* data) {

		kml_base_log(__FILE__, "isr($U) -> $P\n", (unsigned) vector, data);
}
