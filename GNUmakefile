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

include test/lib/base/base.mk

.DEFAULT_GOAL := all

.PHONY: all
all: kernel/kernel.target.out tool/writer.host.out test/lib/base.host.out

.PHONY: test
test: test_base

.PHONY: clean
clean: clean_image clean_kernel clean_base clean_fat clean_writer clean_test_base

