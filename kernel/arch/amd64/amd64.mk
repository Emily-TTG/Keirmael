# SPDX-Licence-Identifier: X11
# Copyright (C) 2025 Emily "TTG" Banerjee <prs.ttg+kml@pm.me>

KERNEL_CFLAGS += -mcmodel=kernel -ffreestanding
KERNEL_LDFLAGS += -e _start

KERNEL_LDSCRIPT = kernel/arch/amd64/linker.ld
