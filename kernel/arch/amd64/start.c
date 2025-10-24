// SPDX-Licence-Identifier: X11
// Copyright (C) 2025 Emily "TTG" Banerjee <prs.ttg+kml@pm.me>

#include <kml/base/environment.h>
#include <kml/base/log.h>

#include <kml/kernel/arch/amd64/table.h>

#include <ultra_protocol.h>

// TODO: Once we have more formal kernel terminal drivers -- this can be
//		 banished to `lib/base/target` to call through those interfaces rather
//		 than this janky debug solution.
enum kml_base_result kml_base_log_character(int character) {
	KML_BASE_ASM("outb %[data], $0xE9" ::[data]"a"((char) character));

	return KML_BASE_RESULT_OK;
}

void kml_kernel_arch_amd64_start(
		struct ultra_boot_context* boot_context, uint32_t magic) {

	(void) boot_context;
	(void) magic;

	KML_BASE_ASM("cli");

	if(magic != ULTRA_MAGIC) {
		kml_base_log_result(
				__FILE__, KML_BASE_RESULT_ERROR_INVALID_PARAMETER,
				"kml_kernel_arch_amd64_gdt_load()");
	}

	enum kml_base_result result = kml_kernel_arch_amd64_gdt_load();
	if(result) {
		kml_base_log_result(
				__FILE__, result, "kml_kernel_arch_amd64_gdt_load()");
	}

	kml_base_log(__FILE__, "Boot done.\n");

	kml_base_log_result(
			__FILE__, KML_BASE_RESULT_ERROR_INVALID_CONTROL_PATH,
			"kml_kernel_arch_amd64_start($P, $X)", boot_context, magic);
}
