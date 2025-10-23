# SPDX-Licence-Identifier: X11
# Copyright (C) 2025 Emily "TTG" Banerjee <prs.ttg+kml@pm.me>

GLOBAL_CFLAGS = -std=c23

ifdef MAINTAINER
	GLOBAL_CFLAGS += -Wall -Werror -Wextra -pedantic -pedantic-errors
endif

ifdef DEBUG
	GLOBAL_CFLAGS += -glldb
else
	GLOBAL_CFLAGS += -DNDEBUG
endif

GLOBAL_CFLAGS += $(CFLAGS)
