# SPDX-Licence-Identifier: X11
# Copyright (C) 2025 Emily "TTG" Banerjee <prs.ttg+kml@pm.me>

TARGET_CC = clang
TARGET_AR = llvm-ar
TARGET_LD = ld.lld

TARGET_CFLAGS = -fPIC --target=x86_64-unknown-elf
TARGET_LDFLAGS =
