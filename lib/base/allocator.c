// SPDX-Licence-Identifier: X11
// Copyright (C) 2025 Emily "TTG" Banerjee <prs.ttg+kml@pm.me>

#include <kml/base/allocator.h>
#include <kml/base/bitset.h>
#include <kml/base/type.h>
#include <kml/base/result.h>

enum kml_base_result kml_base_allocator_region_new(
		struct kml_base_allocator_region* head, kml_base_byte_t* new, kml_base_size_t size,
		kml_base_size_t block_size) {

	if(size < sizeof(struct kml_base_allocator_region)) [[clang::unlikely]] {
		return KML_BASE_RESULT_ERROR_BUFFER_TOO_SMALL;
	}

	if((head && head->block_size == 0) || block_size == 0) [[clang::unlikely]] {
		return KML_BASE_RESULT_ERROR_OUT_OF_RANGE;
	}

	struct kml_base_allocator_region* new_region = (struct kml_base_allocator_region*) new;
	new_region->free = 0;
	new_region->total = 0;
	new_region->next = 0;
	new_region->size = size;

	if(head) {
		new_region->block_size = head->block_size;

		while(head->next) head = head->next;
		head->next = new_region;
	}
	else new_region->block_size = block_size;

	kml_base_size_t start = sizeof(struct kml_base_allocator_region);
	kml_base_size_t bit = 0;
	new_region->tail = size % new_region->block_size;
	kml_base_size_t end = size - new_region->tail;

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

void kml_base_allocator_region_get_statistics(
		struct kml_base_allocator_region* head, struct kml_base_allocator_statistics* out) {

	*out = (struct kml_base_allocator_statistics) {};

	out->block_size = head->block_size;

	do {
		out->free += head->free;
		out->total += head->total;

		if(out->max_contiguous_free < head->free) {
			kml_base_size_t contiguous = 0;

			for(kml_base_size_t i = 0; i < head->total; ++i) {
				if(!kml_base_bitset_get(head->data, i)) {
					if(++contiguous > out->max_contiguous_free) {
						out->max_contiguous_free = contiguous;
					}
				}
				else contiguous = 0;
			}
		}
	} while((head = head->next));
}

static kml_base_byte_t* kml_base_allocator_region_get_block(
		struct kml_base_allocator_region* region, kml_base_size_t index) {

	return region->data + (region->size - sizeof(struct kml_base_allocator_region)) - ((region->block_size * (index + 1)) + region->tail);
}

enum kml_base_result kml_base_allocator_allocation_new(
		struct kml_base_allocator_region* head, struct kml_base_allocator_allocation* out,
		kml_base_size_t count) {

	*out = (struct kml_base_allocator_allocation) {};
	out->count = count;
	out->region = head;

	do {
		if(out->region->free < count) continue;

		kml_base_size_t contiguous = 0;
		for(out->index = 0; out->index < out->region->total; ++out->index) {
			if(!kml_base_bitset_get(out->region->data, out->index)) {
				if(++contiguous >= count) {
					for(kml_base_size_t i = 0; i < count; ++i) {
						kml_base_bitset_set(out->region->data, out->index - i, KML_BASE_BOOL_TRUE);
					}

					out->region->free -= count;
					out->buffer = kml_base_allocator_region_get_block(out->region, out->index);

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
	if(!result) [[clang::likely]] *out = allocation.buffer;

	return result;
}

enum kml_base_result kml_base_allocator_block_delete(
		struct kml_base_allocator_region* head, kml_base_byte_t* block) {

	do {
		kml_base_byte_t* last = kml_base_allocator_region_get_block(head, 0);
		kml_base_byte_t* first = kml_base_allocator_region_get_block(head, head->total - 1);

		if(block >= first && block <= last) {
			kml_base_size_t index = (last - block) / head->block_size;

			if(!kml_base_bitset_get(head->data, index)) [[clang::unlikely]] {
				return KML_BASE_RESULT_ERROR_PARAMETER_LIFETIME_OVER;
			}

			kml_base_bitset_set(head->data, index, KML_BASE_BOOL_FALSE);

			return KML_BASE_RESULT_OK;
		}
	} while((head = head->next));

	return KML_BASE_RESULT_ERROR_OUT_OF_RANGE;
}
