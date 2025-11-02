// SPDX-Licence-Identifier: X11
// Copyright (C) 2025 Emily "TTG" Banerjee <prs.ttg+kml@pm.me>

#include <kml/base/allocator.h>
#include <kml/base/bitset.h>
#include <kml/base/type.h>
#include <kml/base/result.h>

enum kml_base_result kml_base_allocator_region_new(
		struct kml_base_allocator_region* head, void* new, kml_base_size_t size,
		kml_base_size_t block_size) {

	if(size < sizeof(struct kml_base_allocator_region)) {
		return KML_BASE_RESULT_ERROR_BUFFER_TOO_SMALL;
	}

	if((head && head->block_size == 0) || block_size == 0) {
		return KML_BASE_RESULT_ERROR_OUT_OF_RANGE;
	}

	struct kml_base_allocator_region* new_region = new;
	new_region->free = 0;
	new_region->total = 0;
	new_region->next = 0;
	new_region->size = size;

	if(head) {
		new_region->block_size = head->block_size;

		while(head->next) head = head->next;
		head->next = new;
	}
	else new_region->block_size = block_size;

	kml_base_size_t start = sizeof(struct kml_base_allocator_region);
	kml_base_size_t bit = 0;
	kml_base_size_t end = size;

	while(start < end - new_region->block_size) {
		kml_base_bitset_set(new_region->data, bit++, KML_BASE_BOOL_FALSE);
		start = sizeof(struct kml_base_allocator_region) + (bit / KML_BASE_BYTE_BIT);

		if(end < new_region->block_size) break;

		end -= new_region->block_size;
		new_region->total++;
		new_region->free++;
	}

	return new_region->total < 1 ? KML_BASE_RESULT_ERROR_BUFFER_TOO_SMALL : KML_BASE_RESULT_OK;
}


enum kml_base_result kml_base_allocator_allocation_new(
		struct kml_base_allocator_region* head, struct kml_base_allocator_allocation* out,
		kml_base_size_t count) {

	*out = (struct kml_base_allocator_allocation) {};
	out->count = count;
	out->region = head;

	do {
		if(head->free < count) continue;

		kml_base_size_t contiguous = 0;
		for(out->index = 0; out->index < out->region->total; ++out->index) {
			if(!kml_base_bitset_get(out->region->data, out->index)) {
				if(++contiguous >= count) {
					for(kml_base_size_t i = 0; i < count; ++i) {
						kml_base_bitset_set(out->region->data, out->index - i, KML_BASE_BOOL_TRUE);
					}
					out->region->free -= count;
					out->buffer = out->region->data + (out->region->size - sizeof(struct kml_base_allocator_region)) - (out->region->block_size * (out->index + 1));

					return KML_BASE_RESULT_OK;
				}
			}
			else contiguous = 0;
		}
	} while((out->region = out->region->next));

	return KML_BASE_RESULT_ERROR_OUT_OF_MEMORY;
}

void kml_base_allocator_allocation_delete(struct kml_base_allocator_allocation* allocation) {
	for(kml_base_size_t i = 0; i < allocation->count; ++i) {
		kml_base_bitset_set(allocation->region->data, allocation->index - i, KML_BASE_BOOL_FALSE);
	}

	allocation->region->free += allocation->count;

	*allocation = (struct kml_base_allocator_allocation) {};
}

enum kml_base_result kml_base_allocator_block_new(
		struct kml_base_allocator_region* head, kml_base_byte_t** out) {

	struct kml_base_allocator_allocation allocation = {};

	enum kml_base_result result = kml_base_allocator_allocation_new(head, &allocation, 1);
	if(!result) *out = allocation.buffer;

	return result;
}

enum kml_base_result kml_base_allocator_block_delete(
		struct kml_base_allocator_region* head, kml_base_byte_t* block) {

	do {
		kml_base_byte_t* last = head->data + (head->size - sizeof(struct kml_base_allocator_region));
		kml_base_byte_t* first = last - (head->block_size * (head->total + 1));

		if(block >= first && block <= last) {
			kml_base_size_t index = (block - first) / head->block_size;
			kml_base_bitset_set(head->data, index, KML_BASE_BOOL_FALSE);

			return KML_BASE_RESULT_OK;
		}
	} while((head = head->next));

	return KML_BASE_RESULT_ERROR_OUT_OF_RANGE;
}
