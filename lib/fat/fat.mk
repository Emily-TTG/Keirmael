# SPDX-Licence-Identifier: X11
# Copyright (C) 2025 Emily "TTG" Banerjee <prs.ttg+kml@pm.me>

FAT_SOURCES = $(wildcard lib/fat/*.c)

lib/libfat.%.a: $(FAT_SOURCES:.c=.%.o) ; ${$*_library}
