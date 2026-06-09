// SPDX-Licence-Identifier: X11
// Copyright (C) 2025 Emily "TTG" Banerjee <prs.ttg+kml@pm.me>

#include <kml/base/result.h>
#include <kml/base/allocator.h>

#include <kml/base/host/host.h>

#include <kml/kernel/memory.h>

#include <kml/kernel/arch/start.h>

#include <stdio.h>

#include <unistd.h>
#include <sys/mman.h>

kml_base_pointer_t kml_kernel_arch_allocated_physical(kml_base_byte_t* allocated) {
	return (kml_base_pointer_t) allocated;
}

kml_base_byte_t* kml_kernel_arch_physical_allocated(const kml_base_pointer_t physical) {
	return (kml_base_byte_t*) physical;
}

enum kml_base_result kml_kernel_arch_boot_populate_allocator(
		kml_kernel_arch_boot_data_t, struct kml_base_allocator_region**) {

	return KML_BASE_RESULT_OK;
}

enum kml_base_result kml_kernel_arch_boot_map_default(
		kml_kernel_arch_boot_data_t, struct kml_base_allocator_region*, kml_kernel_memory_mapping_context_t) {

	return KML_BASE_RESULT_OK;
}

enum kml_base_result kml_base_main() {
	enum kml_base_result result;

	struct kml_base_allocator_region* region;
	{
		const long page_size = sysconf(_SC_PAGE_SIZE);
		if(page_size == -1) {
			kml_base_host_log_errno(__FILE__, "sysconf(_SC_PAGE_SIZE)");
			return KML_BASE_RESULT_ERROR_INVALID_PARAMETER;
		}

		const kml_base_size_t size = page_size * 1024;
		kml_base_pointer_t base;
		do {
			// We want a low mapping so we can represent resultant allocations in
			// short physical address fields.
			FILE* file = fopen("/proc/sys/vm/mmap_min_addr", "r");
			if(!file) {
				kml_base_host_log_errno(__FILE__, "fopen(\"/proc/sys/vm/mmap_min_addr\", \"r\")");
				base = size;
				break;
			}

			unsigned base_raw;
			if(fscanf(file, "%u", &base_raw) == EOF) {
				kml_base_host_log_errno(__FILE__, "fscanf($P, \"%u\", $P)", file, &base_raw);
				base = size;
			}
			else base = base_raw;

			fclose(file);
		} while(false);

		void* buffer = mmap((void*) base, size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS | MAP_FIXED_NOREPLACE, -1, 0);
		if(!buffer) {
			kml_base_host_log_errno(
					__FILE__,
					"mmap($P, $Z, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS | MAP_FIXED_NOREPLACE, -1, 0)",
					base, size);

			return KML_BASE_RESULT_ERROR_OUT_OF_MEMORY;
		}

		result = kml_base_allocator_region_new(nullptr, buffer, size, KML_KERNEL_PAGE);
		if(result) return result;

		region = buffer;
	}

	kml_kernel_memory_mapping_context_t context;

	result = kml_kernel_memory_mapping_context_new(region, &context);
	if(result) return result;

	result = kml_kernel_memory_mapping_new(
			region,
			context,
			nullptr,
			0,
			0xFFFF'8000'0000'0000,
			20,
			KML_KERNEL_MEMORY_MAPPING_GRANULARITY_4KIB,
			KML_KERNEL_MEMORY_MAPPING_PROTECTION_READ);

	if(result) return result;

	return KML_BASE_RESULT_OK;
}
