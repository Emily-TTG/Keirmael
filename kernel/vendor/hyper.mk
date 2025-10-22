# SPDX-Licence-Identifier: X11
# Copyright (C) 2025 Emily "TTG" Banerjee <prs.ttg+kml@pm.me>

HYPER_DIRECTORY = kernel/vendor/hyper

HYPER_BUILD = $(HYPER_DIRECTORY)/build-clang-$(TARGET)-uefi
HYPER_OUT = $(HYPER_BUILD)/loader/hyper_uefi

HYPER_FLAGS = --platform uefi --arch $(TARGET) --toolchain clang
HYPER_FLAGS += --skip-toolchain-dependencies --skip-base-dependencies
HYPER_FLAGS += --e9-debug-log on

$(HYPER_OUT):
	$(HYPER_DIRECTORY)/build.py $(HYPER_FLAGS)
