# SPDX-Licence-Identifier: X11
# Copyright (C) 2025 Emily "TTG" Banerjee <prs.ttg+kml@pm.me>

.SECONDARY:

include toolchain/global.mk
include toolchain/host.mk
include toolchain/target.mk

include lib/base/base.mk
include lib/fat/fat.mk

include kernel/kernel.mk

include image/image.mk

.DEFAULT_GOAL := all

.PHONY: all
all: kernel/kernel.target.out

.PHONY: clean
clean:
