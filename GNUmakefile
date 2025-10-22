# SPDX-Licence-Identifier: X11
# Copyright (C) 2025 Emily "TTG" Banerjee <prs.ttg+kml@pm.me>

.SECONDARY:

include toolchain/global.mk
include toolchain/host.mk
include toolchain/target.mk

include lib/fat.mk

.PHONY: all
all: lib/libfat.host.a lib/libfat.target.a

.PHONY: clean
clean:
	git clean -fX
