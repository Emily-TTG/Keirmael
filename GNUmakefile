# SPDX-Licence-Identifier: X11
# Copyright (C) 2025 Emily "TTG" Banerjee <prs.ttg+kml@pm.me>

.SECONDARY:

include toolchain/global.mk
include toolchain/host.mk
include toolchain/target.mk

include lib/base/base.mk
include lib/fat/fat.mk

include tool/writer/writer.mk

include kernel/kernel.mk

include image/image.mk

.DEFAULT_GOAL := all

.PHONY: all
all: kernel/kernel.target.out tool/writer.host.out

.PHONY: clean
clean: clean_image clean_kernel clean_base clean_fat clean_writer

