# SPDX-Licence-Identifier: X11
# Copyright (C) 2025 Emily "TTG" Banerjee <prs.ttg+kml@pm.me>

BASE_SOURCES = $(wildcard lib/base/*.c)

lib/libbase.%.a: $(BASE_SOURCES:.c=.%.o)
	${$*_library}
