# SPDX-Licence-Identifier: X11
# Copyright (C) 2025 Emily "TTG" Banerjee <prs.ttg+kml@pm.me>

TARGET_CC = clang
TARGET_AR = llvm-ar

TARGET_CFLAGS = --target=x86_64-unknown-elf -mcmodel=kernel -ffreestanding
