# SPDX-Licence-Identifier: X11
# Copyright (C) 2025 Emily "TTG" Banerjee <prs.ttg+kml@pm.me>

TARGET_CC = clang
TARGET_AR = llvm-ar
TARGET_LD = ld.lld

TARGET_CFLAGS += -fPIC --target=x86_64-unknown-elf -mno-red-zone
TARGET_CFLAGS += -mno-mmx -mno-sse -mno-sse2 -mno-sse3 -mno-3dnow -mgeneral-regs-only
