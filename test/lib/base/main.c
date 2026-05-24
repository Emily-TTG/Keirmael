// SPDX-Licence-Identifier: X11
// Copyright (C) 2025 Emily "TTG" Banerjee <prs.ttg+kml@pm.me>

#include <kml/base/result.h>
#include <kml/base/allocator.h>
#include <kml/base/log.h>
#include <kml/base/type.h>

#include <stdlib.h>
#include <string.h>

static void kml_test_base_allocator_fill_allocation(struct kml_base_allocator_allocation* allocation) {
	for(kml_base_size_t i = 0; i < allocation->count * allocation->region->block_size; ++i) {
		allocation->buffer[i] = (kml_base_byte_t) (allocation->index % 0xFF);
	}
}

static enum kml_base_result kml_test_base_allocator_test_allocation(struct kml_base_allocator_allocation* allocation) {
	for(kml_base_size_t i = 0; i < allocation->count * allocation->region->block_size; ++i) {
		if(allocation->buffer[i] != (kml_base_byte_t) (allocation->index % 0xFF)) {
			return KML_BASE_RESULT_ERROR_INTEGRITY_TEST_FAILED;
		}
	}

	return KML_BASE_RESULT_OK;
}

static void kml_test_base_allocator_fill_block(kml_base_byte_t* block, kml_base_size_t block_size) {
	for(kml_base_size_t i = 0; i < block_size; ++i) {
		block[i] = (kml_base_byte_t) ((kml_base_pointer_t) block % 0xFF);
	}
}

static enum kml_base_result kml_test_base_allocator_test_block(kml_base_byte_t* block, kml_base_size_t block_size) {
	for(kml_base_size_t i = 0; i < block_size; ++i) {
		if(block[i] != (kml_base_byte_t) ((kml_base_pointer_t) block % 0xFF)) return KML_BASE_RESULT_ERROR_INTEGRITY_TEST_FAILED;
	}

	return KML_BASE_RESULT_OK;
}

enum kml_base_result kml_base_main(void) {
	static const kml_base_size_t block_sizes[] = {
		1, 10, 16, 17, 100, 101, 1023, 1024, 1025, 4096
	};

	struct kml_test_base_allocator_allocation {
		kml_base_size_t size;
		struct kml_base_allocator_allocation allocation;
		enum kml_base_result result;
	} allocations[] = {
		{ .size = 1 },
		{ .size = 2 },
		{ .size = 3 },
		{ .size = 5 },
		{ .size = 10 },
		{ .size = 15 },
		{ .size = 16 },
		{ .size = 17 },
		{ .size = 20 },
		{ .size = 30 },
		{ .size = 32 },
		{ .size = 33 }
	};

	enum kml_base_result test_result = KML_BASE_RESULT_OK;

	for(kml_base_size_t i = 0; i < KML_BASE_LENGTH(block_sizes) * 2; ++i) {
		kml_base_size_t block = block_sizes[i % KML_BASE_LENGTH(block_sizes)];

		kml_base_size_t a = block;
		void* region_a = calloc(1, a);

		kml_base_size_t b = block * 13 + 13;
		void* region_b = calloc(1, b);

		kml_base_size_t c = block * 20;
		void* region_c = calloc(1, c);

		kml_base_size_t d = block * 32 + 16;
		void* region_d = calloc(1, d);

		kml_base_size_t e = block * block * 32;
		void* region_e = calloc(1, e);

		if(i >= KML_BASE_LENGTH(block_sizes)) {
			kml_base_log(__FILE__, "=== Allocator Test; Block Size $Z; Non-Zeroed Regions ===\n", block);

			memset(region_a, 0xFF, a);
			memset(region_b, 0xFF, b);
			memset(region_c, 0xFF, c);
			memset(region_d, 0xFF, d);
			memset(region_e, 0xFF, e);
		}
		else kml_base_log(__FILE__, "=== Allocator Test; Block Size $Z; Zeroed Regions ===\n", block);

		kml_base_log(__FILE__, "\t== New Regions; Independent ==\n", block);

		enum kml_base_result result = kml_base_allocator_region_new(0, region_a, a, block);
		kml_base_log_result(__FILE__, result, "\t\tkml_base_allocator_region_new(0, $P<a>, $Z)", region_a, a);
		result = kml_base_allocator_region_new(0, region_b, b, block);
		kml_base_log_result(__FILE__, result, "\t\tkml_base_allocator_region_new(0, $P<b>, $Z)", region_b, b);
		result = kml_base_allocator_region_new(0, region_c, c, block);
		kml_base_log_result(__FILE__, result, "\t\tkml_base_allocator_region_new(0, $P<c>, $Z)", region_c, c);
		result = kml_base_allocator_region_new(0, region_d, d, block);
		kml_base_log_result(__FILE__, result, "\t\tkml_base_allocator_region_new(0, $P<d>, $Z)", region_d, d);
		result = kml_base_allocator_region_new(0, region_e, e, block);
		kml_base_log_result(__FILE__, result, "\t\tkml_base_allocator_region_new(0, $P<e>, $Z)", region_e, e);

		kml_base_log(__FILE__, "\t== New Regions; Chain ==\n", block);

		result = kml_base_allocator_region_new(0, region_a, a, block);
		kml_base_log_result(__FILE__, result, "\t\tkml_base_allocator_region_new($P, $P<a>, $Z) -> $Z blocks", 0, region_a, a, ((struct kml_base_allocator_region*) region_a)->free);
		struct kml_base_allocator_region* head = !result ? region_a : 0;
		result = kml_base_allocator_region_new(head, region_b, b, block);
		kml_base_log_result(__FILE__, result, "\t\tkml_base_allocator_region_new($P, $P<b>, $Z) -> $Z blocks", head, region_b, b, ((struct kml_base_allocator_region*) region_b)->free);
		head = !result ? (head ? head : region_b) : 0;
		result = kml_base_allocator_region_new(head, region_c, c, block);
		kml_base_log_result(__FILE__, result, "\t\tkml_base_allocator_region_new($P, $P<c>, $Z) -> $Z blocks", head, region_c, c, ((struct kml_base_allocator_region*) region_c)->free);
		head = !result ? (head ? head : region_c) : 0;
		result = kml_base_allocator_region_new(head, region_d, d, block);
		kml_base_log_result(__FILE__, result, "\t\tkml_base_allocator_region_new($P, $P<d>, $Z) -> $Z blocks", head, region_d, d, ((struct kml_base_allocator_region*) region_d)->free);
		head = !result ? (head ? head : region_d) : 0;
		result = kml_base_allocator_region_new(head, region_e, e, block);
		kml_base_log_result(__FILE__, result, "\t\tkml_base_allocator_region_new($P, $P<e>, $Z) -> $Z blocks", head, region_e, e, ((struct kml_base_allocator_region*) region_e)->free);
		head = !result ? (head ? head : region_e) : 0;

		kml_base_log(__FILE__, "\t== Allocations; Concurrent ==\n", block);

		for(kml_base_size_t j = 0; head && j < KML_BASE_LENGTH(allocations); ++j) {
			kml_base_byte_t* allocated_block = 0;
			result = kml_base_allocator_block_new(head, &allocated_block);
			kml_base_log_result(
				__FILE__, result, "\t\tkml_base_allocator_block_new($P, $P) -> $P", head, &allocated_block, allocated_block);

			if(allocated_block) kml_test_base_allocator_fill_block(allocated_block, head->block_size);

			allocations[j] = (struct kml_test_base_allocator_allocation) {};
			allocations[j].result = result = kml_base_allocator_allocation_new(head, &allocations[j].allocation, allocations[j].size);
			kml_base_log_result(
				__FILE__, result, "\t\tkml_base_allocator_allocation_new($P, $P<$Z>, $Z) -> { .buffer = $P, .index = $Z }",
				head, &allocations[j], j, allocations[j].size, allocations[j].allocation.buffer, allocations[j].allocation.index);

			if(!result) kml_test_base_allocator_fill_allocation(&allocations[j].allocation);

			if(allocated_block) {
				result = kml_test_base_allocator_test_block(allocated_block, head->block_size);
				kml_base_log_result(
					__FILE__, result, "\t\tkml_test_base_allocator_test_block($P, $Z)", allocated_block, head->block_size);

				if(result) [[clang::unlikely]] {
					kml_base_log_result(
						__FILE__, KML_BASE_RESULT_ERROR_UNIT_TEST_FAILED,
						"\t\t\tkml_test_base_allocator_test_block($P, $Z) != $R", allocated_block, head->block_size, KML_BASE_RESULT_OK);

					test_result = KML_BASE_RESULT_ERROR_UNIT_TEST_FAILED;
				}

				result = kml_base_allocator_block_delete(head, allocated_block);
				kml_base_log_result(
					__FILE__, result, "\t\tkml_base_allocator_block_delete($P, $P)", head, allocated_block);
			}
		}

		for(kml_base_size_t j = 0; head && j < KML_BASE_LENGTH(allocations); ++j) {
			if(!allocations[j].result) {
				result = kml_test_base_allocator_test_allocation(&allocations[j].allocation);
				kml_base_log_result(
					__FILE__, result, "\t\tkml_test_base_allocator_test_allocation($P<$Z>)", &allocations[j], j);

				if(result) [[clang::unlikely]] {
					kml_base_log_result(
						__FILE__, KML_BASE_RESULT_ERROR_UNIT_TEST_FAILED,
						"\t\t\tkml_test_base_allocator_test_allocation($P<$Z>) != $R", &allocations[j], j, KML_BASE_RESULT_OK);

					test_result = KML_BASE_RESULT_ERROR_UNIT_TEST_FAILED;
				}

				kml_base_allocator_allocation_delete(&allocations[j].allocation);
			}
		}

		kml_base_log(__FILE__, "\t== Allocations; Sequential ==\n", block);

		for(kml_base_size_t j = 0; head && j < KML_BASE_LENGTH(allocations); ++j) {
			result = kml_base_allocator_allocation_new(head, &allocations[j].allocation, allocations[j].size);
			kml_base_log_result(
				__FILE__, result, "\t\tkml_base_allocator_allocation_new($P, $P<$Z>, $Z) -> { .buffer = $P, .index = $Z }",
				head, &allocations[j], j, allocations[j].size, allocations[j].allocation.buffer, allocations[j].allocation.index);

			if(result && result != allocations[j].result) {
				kml_base_log_result(
					__FILE__, KML_BASE_RESULT_ERROR_UNIT_TEST_FAILED,
					"\t\t\tresult ($R) != allocations[$Z].result ($R)", result, j, allocations[j].result);

				test_result = KML_BASE_RESULT_ERROR_UNIT_TEST_FAILED;
			}

			if(!result) {
				kml_test_base_allocator_fill_allocation(&allocations[j].allocation);

				result = kml_test_base_allocator_test_allocation(&allocations[j].allocation);
				kml_base_log_result(
					__FILE__, result, "\t\tkml_test_base_allocator_test_allocation($P<$Z>)", &allocations[j], j);

				if(result) [[clang::unlikely]] {
					kml_base_log_result(
						__FILE__, KML_BASE_RESULT_ERROR_UNIT_TEST_FAILED,
						"\t\tkml_test_base_allocator_test_allocation($P<$Z>) != $R", j, &allocations[j], j, KML_BASE_RESULT_OK);

					test_result = KML_BASE_RESULT_ERROR_UNIT_TEST_FAILED;
				}

				kml_base_allocator_allocation_delete(&allocations[j].allocation);
			}
		}

		free(region_e);
		free(region_d);
		free(region_c);
		free(region_b);
		free(region_a);
	}

	return test_result;
}
