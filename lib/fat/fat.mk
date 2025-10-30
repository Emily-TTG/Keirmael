# SPDX-Licence-Identifier: X11
# Copyright (C) 2025 Emily "TTG" Banerjee <prs.ttg+kml@pm.me>

FAT_SOURCES = $(wildcard lib/fat/*.c)

lib/libfat.%.a: INCLUDES = lib/fat/include lib/base/include
lib/libfat.%.a: $(FAT_SOURCES:.c=.%.o)
	${$*_library}

.PHONY: clean_fat
clean_fat:
	rm -f $(FAT_SOURCES:.c=.host.o)
	rm -f lib/libfat.host.a

	rm -f $(FAT_SOURCES:.c=.target.o)
	rm -f lib/libfat.target.a
