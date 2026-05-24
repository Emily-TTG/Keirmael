// SPDX-Licence-Identifier: X11
// Copyright (C) 2025 Emily "TTG" Banerjee <prs.ttg+kml@pm.me>

#include <kml/base/allocator.h>
#include <kml/base/memory.h>

#include <kml/kernel/memory.h>

#include <kml/kernel/arch/start.h>

#include <kml/kernel/arch/amd64/table.h>

enum kml_base_result kml_kernel_memory_mapping_context_new(
		struct kml_base_allocator_region* allocator, kml_kernel_memory_mapping_context_t* out) {

	kml_base_byte_t* allocated;

	const enum kml_base_result result = kml_base_allocator_block_new(allocator, &allocated);
	if(result) [[clang::unlikely]] return result;

	*out = allocated;

	kml_base_memory_fill(*out, allocator->block_size, 0);

	return KML_BASE_RESULT_OK;
}

enum kml_base_result kml_kernel_memory_mapping_context_load(kml_kernel_memory_mapping_context_t context) {
	KML_BASE_ASM("movq %[table], %%cr3" :: [table]"a"(context) : "cr3");

	return KML_BASE_RESULT_OK;
}

enum kml_base_result kml_kernel_memory_mapping_new(
		struct kml_base_allocator_region* allocator, const kml_kernel_memory_mapping_context_t context, struct kml_kernel_memory_mapping* out,
		const kml_base_pointer_t physical, const kml_base_pointer_t virtual, const kml_base_size_t count,
		const enum kml_kernel_memory_mapping_granularity granularity,
		const enum kml_kernel_memory_mapping_protection protection) {

	static constexpr kml_base_pointer_t offset_mask = 0xFFF;
	static constexpr kml_base_pointer_t offset_count = KML_BASE_POPCOUNT(offset_mask);
	static constexpr kml_base_pointer_t level_mask = 0x1FF;
	static constexpr kml_base_pointer_t level_count = KML_BASE_POPCOUNT(level_mask);

	if(virtual & offset_mask || physical & offset_mask) [[clang::unlikely]] return KML_BASE_RESULT_ERROR_PARAMETER_NOT_ALIGNED;
	// TODO: Test for non-canonical virtual address.

	// TODO: Free any branches allocated by this mapping attempt on failure.

	if(out) {
		out->count = count;
		out->granularity = granularity;
		out->mapped = (kml_base_byte_t*) virtual;
	}

	const kml_base_size_t page_size = (kml_base_size_t) 1 << (offset_count + (granularity * level_count));

	constexpr kml_base_size_t canonical_shift = (sizeof(kml_base_pointer_t) * KML_BASE_BYTE_BIT) - (offset_count + (KML_KERNEL_ARCH_AMD64_PML_MAX * level_count));

	for(kml_base_size_t page = 0; page < count; ++page) {
		const kml_base_pointer_t page_physical = physical + page * page_size;
		kml_base_pointer_t page_virtual = (virtual + page * page_size) << canonical_shift;
		page_virtual >>= canonical_shift;

		struct kml_kernel_arch_amd64_page_table_entry* head = context;
		struct kml_kernel_arch_amd64_page_table_entry* entry = nullptr;

		for(kml_base_size_t i = KML_KERNEL_ARCH_AMD64_PML_MAX - 1; i > 0; --i) {
			const kml_base_size_t index = (page_virtual >> (offset_count + (i * level_count))) & level_mask;

			entry = &head[index];
			if(entry->present) {
				if(i == granularity) [[clang::unlikely]] return KML_BASE_RESULT_ERROR_VALUE_ALREADY_SET;
			}
			else {
				if(i == granularity) break;

				kml_base_byte_t* allocated;
				const enum kml_base_result result = kml_base_allocator_block_new(allocator, &allocated);
				if(result) [[clang::unlikely]] return result;

				kml_base_memory_fill(allocated, allocator->block_size, 0);

				entry->present = KML_BASE_BOOL_TRUE;
				entry->writeable = KML_BASE_BOOL_TRUE;
				entry->address = kml_kernel_arch_allocated_physical(allocated) >> offset_count;
			}

			head = (struct kml_kernel_arch_amd64_page_table_entry*) kml_kernel_arch_physical_allocated((kml_base_pointer_t) entry->address << offset_count);
			entry = nullptr;
		}

		if(!entry) {
			const kml_base_size_t index = (page_virtual >> offset_count) & level_mask;
			entry = &head[index];
		}

		if(granularity != KML_KERNEL_MEMORY_MAPPING_GRANULARITY_4KIB) {
			entry->large_pat = KML_BASE_BOOL_TRUE;
		}

		entry->present = KML_BASE_BOOL_TRUE;
		entry->writeable = !!(protection & KML_KERNEL_MEMORY_MAPPING_PROTECTION_WRITE);
		entry->disable_execute = !(protection & KML_KERNEL_MEMORY_MAPPING_PROTECTION_EXECUTE);
		entry->address = page_physical >> offset_count;
		// TODO: Set global on kernel maps.
	}

	return KML_BASE_RESULT_OK;
}

enum kml_base_result kml_kernel_memory_mapping_delete(struct kml_kernel_memory_mapping*) {
	return KML_BASE_RESULT_OK;
}
