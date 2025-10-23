// SPDX-Licence-Identifier: X11
// Copyright (C) 2025 Emily "TTG" Banerjee <prs.ttg+kml@pm.me>

#include <kml/kernel/arch/amd64/table.h>

#include <ultra_protocol.h>

static inline void debug_char(char c) {
    __asm__("outb %[data], $0xE9" ::[data]"a"(c));
}

void kml_kernel_arch_amd64_start(
		struct ultra_boot_context* boot_context, uint32_t magic) {

	__asm__("cli");

	(void) boot_context;
	(void) magic;

	for(const char* s = "Hello, world!\n"; *s; ++s) debug_char(*s);

	(void) kml_kernel_arch_amd64_gdt_load();

	for(const char* s = "Done GDT\n"; *s; ++s) debug_char(*s);
}
