// SPDX-Licence-Identifier: X11
// Copyright (C) 2025 Emily "TTG" Banerjee <prs.ttg+kml@pm.me>

#include <kml/base/environment.h>
#include <kml/base/log.h>
#include <kml/base/type.h>
#include <kml/base/allocator.h>

#include <kml/kernel/arch/start.h>

#include <ultra_protocol.h>

[[noreturn]]
void kml_kernel_arch_ultra_start(
		struct ultra_boot_context* boot_context, uint32_t magic) {

	if(magic != ULTRA_MAGIC) {
		kml_base_log_result(
				__FILE__, KML_BASE_RESULT_ERROR_INVALID_PARAMETER,
				"kml_kernel_arch_ultra_start($P, $X) $X != $X",
				boot_context, magic, magic, ULTRA_MAGIC);
	}

	kml_kernel_arch_start(boot_context);
}

enum kml_base_result kml_kernel_arch_process_boot_data(void* boot_data, struct kml_base_allocator_region** allocator) {
	struct ultra_boot_context* boot_context = boot_data;

	*allocator = 0;

	struct ultra_attribute_header* attribute = boot_context->attributes;
	for(kml_base_size_t i = 0; i < boot_context->attribute_count; ++i) {
		switch(attribute->type) {
			default: {
				kml_base_log_result(
					__FILE__, KML_BASE_RESULT_ERROR_NOT_IMPLEMENTED,
					"kml_kernel_arch_process_boot_data($P, $P) boot attribute $X",
					boot_data, allocator, attribute->type);

				break;
			}

			case ULTRA_ATTRIBUTE_MEMORY_MAP: {
				struct ultra_memory_map_attribute* map = (struct ultra_memory_map_attribute*) attribute;

				for(kml_base_size_t j = 0; j < ULTRA_MEMORY_MAP_ENTRY_COUNT(*attribute); ++j) {
					struct ultra_memory_map_entry* entry = &map->entries[j];

					kml_base_log(
						__FILE__, "Memory map entry: type $X, range $P->$P\n",
						entry->type, entry->physical_address, entry->physical_address + entry->size);

					switch(entry->type) {
						default: {
							kml_base_log_result(
								__FILE__, KML_BASE_RESULT_ERROR_NOT_IMPLEMENTED,
								"kml_kernel_arch_process_boot_data($P, $P)",
								boot_context, allocator);

							break;
						}

						/// TODO: Hold reclaimable pages and add to allocator after final attribute is processed.
						case ULTRA_MEMORY_TYPE_FREE: {
							kml_base_pointer_t base = entry->physical_address;
							if(!base && entry->size > KML_KERNEL_PAGE) {
								base += KML_KERNEL_PAGE;

								kml_base_log(__FILE__, "Skipping zero page in free memory range\n");
							}

							enum kml_base_result result = kml_base_allocator_region_new(
								*allocator, (kml_base_byte_t*) base, entry->size, KML_KERNEL_PAGE);

							if(result) {
								kml_base_log_result(
									__FILE__, result, "kml_base_allocator_region_new($P, $P, $Z, $Z)",
									allocator, base, entry->size, KML_KERNEL_PAGE);
							}

							if(!*allocator) *allocator = (struct kml_base_allocator_region*) entry->physical_address;

							break;
						}
					}
				}

				break;
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
