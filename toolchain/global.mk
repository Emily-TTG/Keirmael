# SPDX-Licence-Identifier: X11
# Copyright (C) 2025 Emily "TTG" Banerjee <prs.ttg+kml@pm.me>

GLOBAL_CFLAGS = $(CFLAGS) -std=c17

ifdef MAINTAINER
	GLOBAL_CFLAGS += -Wall -Werror -Wextra -pedantic -pedantic-errors
endif
