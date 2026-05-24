# SPDX-Licence-Identifier: X11
# Copyright (C) 2025 Emily "TTG" Banerjee <prs.ttg+kml@pm.me>

KERNEL_INCLUDES += kernel/arch/amd64/include

KERNEL_CFLAGS += -DKML_KERNEL_PAGE=4096
# TODO: This should be a runtime behaviour inherited from the bootloader config.
KERNEL_CFLAGS += -DKML_KERNEL_ARCH_AMD64_PML_MAX=4

KERNEL_LDSCRIPT = kernel/arch/amd64/linker.ld
