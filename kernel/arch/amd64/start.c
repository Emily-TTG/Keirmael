// SPDX-Licence-Identifier: X11
// Copyright (C) 2025 Emily "TTG" Banerjee <prs.ttg+kml@pm.me>

#include <kml/base/environment.h>
#include <kml/base/log.h>
#include <kml/base/type.h>
#include <kml/base/allocator.h>

#include <kml/kernel/arch/start.h>
#include <kml/kernel/arch/amd64/table.h>

// TODO: Once we have more formal kernel terminal drivers -- this can be
//		 banished to `lib/base/target` to call through those interfaces rather
//		 than this janky debug solution.
enum kml_base_result kml_base_log_character(int character) {
	KML_BASE_ASM("outb %[data], $0xE9" :: [data]"a"((char) character));

	return KML_BASE_RESULT_OK;
}

void kml_kernel_arch_start(void* boot_data) {
	enum kml_base_result result = kml_kernel_arch_amd64_gdt_load();
	if(result) {
		kml_base_log_result(
				__FILE__, result, "kml_kernel_arch_amd64_gdt_load()");
	}

	result = kml_kernel_arch_amd64_idt_load();
	if(result) {
		kml_base_log_result(
				__FILE__, result, "kml_kernel_arch_amd64_idt_load()");
	}

	struct kml_base_allocator_region* allocator;
	result = kml_kernel_arch_process_boot_data(boot_data, &allocator);
	if(result) {
		kml_base_log_result(
				__FILE__, result, "kml_kernel_arch_process_boot_data($P, $P)",
				boot_data, &allocator);
	}

	kml_base_log_result(
			__FILE__, KML_BASE_RESULT_ERROR_INVALID_CONTROL_PATH,
			"kml_kernel_arch_amd64_start($P)", boot_data);

	volatile enum kml_base_bool halt = KML_BASE_BOOL_TRUE;
	while(halt) {
		KML_BASE_ASM("cli");
		KML_BASE_ASM("hlt");
	}

	KML_BASE_UNREACHABLE();
}
