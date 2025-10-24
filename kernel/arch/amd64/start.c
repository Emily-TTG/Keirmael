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

	KML_BASE_ASM("cli");

	(void) boot_context;
	(void) magic;

	kml_base_log(__FILE__, "Hello, world!\n");

	(void) kml_kernel_arch_amd64_gdt_load();

	kml_base_log(__FILE__, "Boot done.\n");
}
