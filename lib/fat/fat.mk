# SPDX-Licence-Identifier: X11
# Copyright (C) 2025 Emily "TTG" Banerjee <prs.ttg+kml@pm.me>

FAT_SOURCES = $(wildcard lib/fat/*.c)
FAT_HEADERS = $(wildcard lib/fat/include/kml/fat/*.h)

lib/libfat.%.a: LOCAL_CFLAGS = -isystem lib/fat/include -isystem lib/base/include
lib/libfat.%.a: $(FAT_SOURCES:.c=.%.o)
	${$*_library}
