# SPDX-Licence-Identifier: X11
# Copyright (C) 2025 Emily "TTG" Banerjee <prs.ttg+kml@pm.me>

KERNEL_CFLAGS += -isystem kernel/arch/amd64/include

KERNEL_CFLAGS += -mcmodel=kernel -ffreestanding
KERNEL_LDFLAGS += -e kml_kernel_arch_amd64_start

KERNEL_LDSCRIPT = kernel/arch/amd64/linker.ld
