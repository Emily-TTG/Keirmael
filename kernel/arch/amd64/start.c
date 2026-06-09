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
#if __STDC_HOSTED__ != 1
enum kml_base_result kml_base_log_character(int character) {
	KML_BASE_ASM("outb %[data], $0xE9" :: [data]"a"((char) character));

	return KML_BASE_RESULT_OK;
}
#endif

void kml_kernel_arch_start(void* boot_data) {
	enum kml_base_result result = kml_kernel_arch_amd64_gdt_load();
	if(result) [[clang::unlikely]] {
		KML_BASE_LOG_RESULT(result, "kml_kernel_arch_amd64_gdt_load()");
		goto terminate;
	}

	result = kml_kernel_arch_amd64_idt_load();
	if(result) [[clang::unlikely]] {
		KML_BASE_LOG_RESULT(result, "kml_kernel_arch_amd64_idt_load()");
		goto terminate;
	}

	struct kml_base_allocator_region* allocator;
	// TODO: Determine failure mode and whether we're safe to proceed with a partial allocator.
	result = kml_kernel_arch_boot_populate_allocator(boot_data, &allocator);
	if(result) [[clang::unlikely]] {
		KML_BASE_LOG_RESULT(result, "kml_kernel_arch_boot_populate_allocator($P, $P)", boot_data, &allocator);
		goto terminate;
	}

	kml_kernel_memory_mapping_context_t mapping_context;
	result = kml_kernel_memory_mapping_context_new(allocator, &mapping_context);
	if(result) [[clang::unlikely]] {
		KML_BASE_LOG_RESULT(result, "kml_kernel_memory_mapping_context_new($P, $P)", allocator, &mapping_context);
		goto terminate;
	}

	result = kml_kernel_arch_boot_map_default(boot_data, allocator, mapping_context);
	if(result) [[clang::unlikely]] {
		KML_BASE_LOG_RESULT(result, "kml_kernel_arch_boot_map_default($P, $P, $P)", boot_data, allocator, mapping_context);
		goto terminate;
	}

	result = kml_kernel_memory_mapping_context_load(mapping_context);
	if(result) [[clang::unlikely]] {
		KML_BASE_LOG_RESULT(result, "kml_kernel_memory_mapping_context_load($P)", mapping_context);
		goto terminate;
	}

	KML_BASE_LOG_RESULT(KML_BASE_RESULT_ERROR_INVALID_CONTROL_PATH, "kml_kernel_arch_amd64_start($P)", boot_data);

terminate:
	KML_BASE_LOG("The kernel has encountered a fatal error and cannot continue\n");

	volatile enum kml_base_bool halt = KML_BASE_BOOL_TRUE;
	while(halt) {
		KML_BASE_ASM("cli");
		KML_BASE_ASM("hlt");
	}

	KML_BASE_UNREACHABLE();
}
