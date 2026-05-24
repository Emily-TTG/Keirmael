// SPDX-Licence-Identifier: X11
// Copyright (C) 2025 Emily "TTG" Banerjee <prs.ttg+kml@pm.me>

#include <kml/base/log.h>
#include <kml/base/type.h>
#include <kml/base/allocator.h>

#include <kml/kernel/memory.h>
#include <kml/kernel/arch/start.h>

#include <ultra_protocol.h>

// TODO: Further subdivide Ultra directory by architecture.
#ifdef __amd64__
# if KML_KERNEL_ARCH_AMD64_PML_MAX == 4
#  define KML_KERNEL_ARCH_ULTRA_DIRECT_MAP_BASE (0xFFFF'8000'0000'0000)
# elif KML_KERNEL_ARCH_AMD64_PML_MAX == 5
#  define KML_KERNEL_ARCH_ULTRA_DIRECT_MAP_BASE (0xFF00'0000'0000'0000)
# endif
#endif

[[noreturn]]
void kml_kernel_arch_ultra_start(
		struct ultra_boot_context* boot_context, const uint32_t magic) {

	if(magic != ULTRA_MAGIC) {
		kml_base_log_result(
				__FILE__, KML_BASE_RESULT_ERROR_INVALID_PARAMETER,
				"kml_kernel_arch_ultra_start($P, $X) $X != $X",
				boot_context, magic, magic, ULTRA_MAGIC);
	}

	kml_kernel_arch_start(boot_context);
}

kml_base_pointer_t kml_kernel_arch_allocated_physical(kml_base_byte_t* allocated) {
	return (kml_base_pointer_t) allocated - KML_KERNEL_ARCH_ULTRA_DIRECT_MAP_BASE;
}

kml_base_byte_t* kml_kernel_arch_physical_allocated(kml_base_pointer_t physical) {
	return (kml_base_byte_t*) (physical + KML_KERNEL_ARCH_ULTRA_DIRECT_MAP_BASE);
}

enum kml_base_result kml_kernel_arch_boot_populate_allocator(
		const kml_kernel_arch_boot_data_t boot_data, struct kml_base_allocator_region** allocator) {

	struct ultra_boot_context* boot_context = boot_data;

	*allocator = nullptr;

	struct ultra_attribute_header* attribute = boot_context->attributes;

	for(kml_base_size_t i = 0; i < boot_context->attribute_count; ++i) {
		if(attribute->type == ULTRA_ATTRIBUTE_MEMORY_MAP) {
			const struct ultra_memory_map_attribute* map = (struct ultra_memory_map_attribute*) attribute;

			for(kml_base_size_t j = 0; j < ULTRA_MEMORY_MAP_ENTRY_COUNT(*attribute); ++j) {
				const struct ultra_memory_map_entry* entry = &map->entries[j];

				switch(entry->type) {
					default: continue;

					/// TODO: Hold reclaimable pages and add to allocator after final attribute is processed.
					case ULTRA_MEMORY_TYPE_FREE: {
						kml_base_pointer_t base = entry->physical_address;
						if(!base && entry->size > KML_KERNEL_PAGE) {
							base += KML_KERNEL_PAGE;

							kml_base_log(__FILE__, "Skipping zero page in free memory range\n");
						}

						base += KML_KERNEL_ARCH_ULTRA_DIRECT_MAP_BASE;

						const enum kml_base_result result = kml_base_allocator_region_new(
							*allocator, (kml_base_byte_t*) base, entry->size, KML_KERNEL_PAGE);

						if(result) [[clang::unlikely]] {
							kml_base_log_result(
								__FILE__, result, "kml_base_allocator_region_new($P, $P, $Z, $Z)",
								allocator, base, entry->size, KML_KERNEL_PAGE);
						}

						if(!*allocator) *allocator = (struct kml_base_allocator_region*) base;

						break;
					}
				}
			}
		}

		attribute = ULTRA_NEXT_ATTRIBUTE(attribute);
	}

	struct kml_base_allocator_statistics allocator_statistics;
	kml_base_allocator_region_get_statistics(*allocator, &allocator_statistics);

	kml_base_log(__FILE__, "Allocator statistics:\n");
	kml_base_log(
		__FILE__, "\tBlock size: $Z\n",
		allocator_statistics.block_size);

	kml_base_log(
		__FILE__, "\tTotal: $Z blocks ($Z B)\n",
		allocator_statistics.total, allocator_statistics.total * allocator_statistics.block_size);

	kml_base_log(
		__FILE__, "\tFree: $Z blocks ($Z B)\n",
		allocator_statistics.free, allocator_statistics.free * allocator_statistics.block_size);

	kml_base_log(
		__FILE__, "\tMax Contiguous Free: $Z blocks ($Z B)\n",
		allocator_statistics.max_contiguous_free, allocator_statistics.max_contiguous_free * allocator_statistics.block_size);

	return KML_BASE_RESULT_OK;
}

enum kml_base_result kml_kernel_arch_boot_map_default(
		const kml_kernel_arch_boot_data_t boot_data, struct kml_base_allocator_region* allocator,
		const kml_kernel_memory_mapping_context_t mapping_context) {

	struct ultra_boot_context* boot_context = boot_data;

	enum kml_base_result result;

	struct ultra_attribute_header* attribute = boot_context->attributes;

	for(kml_base_size_t i = 0; i < boot_context->attribute_count; ++i) {
		if(attribute->type == ULTRA_ATTRIBUTE_MEMORY_MAP) {
			const struct ultra_memory_map_attribute* map = (struct ultra_memory_map_attribute*) attribute;

			for(kml_base_size_t j = 0; j < ULTRA_MEMORY_MAP_ENTRY_COUNT(*attribute); ++j) {
				const struct ultra_memory_map_entry* entry = &map->entries[j];

				kml_base_log(
					__FILE__, "Memory map entry: type $X, range $P->$P\n",
					entry->type, entry->physical_address, entry->physical_address + entry->size);

				// TODO: Be more selective with protection.
				// TODO: Use larger pages when range permits.
				result = kml_kernel_memory_mapping_new(
						allocator, mapping_context, nullptr,
						entry->physical_address, entry->physical_address + KML_KERNEL_ARCH_ULTRA_DIRECT_MAP_BASE,
						entry->size / KML_KERNEL_PAGE, KML_KERNEL_MEMORY_MAPPING_GRANULARITY_4KIB,
						KML_KERNEL_MEMORY_MAPPING_PROTECTION_WRITE | KML_KERNEL_MEMORY_MAPPING_PROTECTION_EXECUTE);

				// TODO: Operate with a memory budget?
				if(result) [[clang::unlikely]] return result;
			}
		}
		else if(attribute->type == ULTRA_ATTRIBUTE_KERNEL_INFO) {
			const struct ultra_kernel_info_attribute* kernel = (struct ultra_kernel_info_attribute*) attribute;

			kml_base_log(
				__FILE__, "Kernel mapping: physical $P, range $P->$P\n",
				kernel->physical_base, kernel->virtual_base, kernel->virtual_base + kernel->size);

			result = kml_kernel_memory_mapping_new(
					allocator, mapping_context, nullptr,
					kernel->physical_base, kernel->virtual_base,
					kernel->size / KML_KERNEL_PAGE, KML_KERNEL_MEMORY_MAPPING_GRANULARITY_4KIB,
					KML_KERNEL_MEMORY_MAPPING_PROTECTION_WRITE | KML_KERNEL_MEMORY_MAPPING_PROTECTION_EXECUTE);

			if(result) [[clang::unlikely]] return result;
		}

		attribute = ULTRA_NEXT_ATTRIBUTE(attribute);
	}

	return KML_BASE_RESULT_OK;
}
