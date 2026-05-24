// SPDX-Licence-Identifier: X11
// Copyright (C) 2025 Emily "TTG" Banerjee <prs.ttg+kml@pm.me>

#pragma once

#include <kml/base/type.h>
#include <kml/base/result.h>

struct kml_base_allocator_region {
	struct kml_base_allocator_region* next;
	kml_base_size_t size; /// The size of the region in bytes, includes this structure.
	kml_base_size_t block_size;
	kml_base_size_t tail; /// The number of unused bytes at the end of the region used to align the blocks to the blocksize.
	kml_base_size_t total; /// The total number of blocks in this region.
	kml_base_size_t free; /// The number of free blocks in this region.
	kml_base_byte_t data[];
};

struct kml_base_allocator_allocation {
	struct kml_base_allocator_region* region;
	kml_base_byte_t* buffer;
	kml_base_size_t count; /// The number of blocks allocated.
	kml_base_size_t index; /// The index of the first allocated block. Blocks are indexed in reverse order from the end of the structure.
};

struct kml_base_allocator_statistics {
	kml_base_size_t block_size;
	kml_base_size_t total; /// The total number of blocks.
	kml_base_size_t free; /// The number of free blocks.
	kml_base_size_t max_contiguous_free; /// The largest contiguous block of free blocks.
};

/**
 * Adds a new free memory region to an allocator region list. Will zero out
 * portions of the acquired region as-necessary.
 *
 * If the provided head is null, the free memory region is transformed in-place
 * into the head of a new allocator region list.
 *
 * \param[in,out,optional] 0 The head of the allocator region list to add the
 * new region to, or null for a new allocator.
 * \param[in] 1 A pointer to the start of the free memory region to add.
 * \param[in] 2 The size of the free memory region to add.
 * \param[in] 3 The block size to use if head is null.
 * \return KML_BASE_RESULT_OK on success.
 * \return KML_BASE_RESULT_ERROR_BUFFER_TOO_SMALL if the free memory region is
 * too small to initialize as part of the allocator region list.
 * \return KML_BASE_RESULT_ERROR_OUT_OF_RANGE if the block size is 0.
 */
enum kml_base_result kml_base_allocator_region_new(
		struct kml_base_allocator_region*, kml_base_byte_t*, kml_base_size_t, kml_base_size_t);

/**
 * Collects statistics from all regions in an allocator region list.
 * \param[in] 0 The head of the allocator region list to inspect.
 * \param[out] 1 A pointer to output storage for the statistics.
 */
void kml_base_allocator_region_get_statistics(
		struct kml_base_allocator_region*, struct kml_base_allocator_statistics*);

/**
 * Allocates a contiguous set of blocks from a region in an allocator region list.
 * The allocation is returned in a descriptive allocation structure
 * \param[in,out] 0 The head of the allocator region list to allocate from.
 * \param[out] 1 A pointer to output storage for the allocation.
 * \param[in] 2 The number of blocks to allocate.
 * \return KML_BASE_RESULT_OK on success.
 * \return KML_BASE_RESULT_ERROR_OUT_OF_MEMORY if insufficient free blocks were available.
 */
enum kml_base_result kml_base_allocator_allocation_new(
		struct kml_base_allocator_region*, struct kml_base_allocator_allocation*,
		kml_base_size_t);

/**
 * Deletes the blocks pointed to by an allocation structure in the allocator region list it was
 * allocated from, and clears the storage of the allocation structure.
 * \param[in,out] 0 The allocation to delete.
 */
void kml_base_allocator_allocation_delete(struct kml_base_allocator_allocation*);

/**
 * Allocates a single block from a region in an allocator region list.
 * \param[in,out] 0 The head of the allocator region list to allocate from.
 * \param[out] 1 A pointer to output storage for the allocation.
 * \return KML_BASE_RESULT_OK on success.
 * \return KML_BASE_RESULT_ERROR_OUT_OF_MEMORY if no free block was available.
 */
enum kml_base_result kml_base_allocator_block_new(
		struct kml_base_allocator_region*, kml_base_byte_t**);

/**
 * Deletes an allocated block from an allocator region list.
 * \param[in,out] 0 The head of the allocator region list to delete from.
 * \param[in] 1 The address of the allocated block to delete.
 * \return KML_BASE_RESULT_OK on success.
 * \return KML_BASE_RESULT_ERROR_OUT_OF_RANGE if the allocated block could not be
 * found in the allocator region list.
 * \return KML_BASE_RESULT_ERROR_PARAMETER_LIFETIME_OVER if the allocated block has
 * already been deleted.
 */
enum kml_base_result kml_base_allocator_block_delete(
		struct kml_base_allocator_region*, kml_base_byte_t*);
