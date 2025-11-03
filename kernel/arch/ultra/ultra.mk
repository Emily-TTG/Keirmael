# SPDX-Licence-Identifier: X11
# Copyright (C) 2025 Emily "TTG" Banerjee <prs.ttg+kml@pm.me>

KERNEL_INCLUDES += kernel/arch/ultra/include

KERNEL_LDFLAGS += -e kml_kernel_arch_ultra_start
