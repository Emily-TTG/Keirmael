# SPDX-Licence-Identifier: X11
# Copyright (C) 2025 Emily "TTG" Banerjee <prs.ttg+kml@pm.me>

WRITER_SOURCES = $(wildcard tool/writer/*.c)

tool/writer.%.out: INCLUDES = lib/fat/include lib/base/include
tool/writer.%.out: $(WRITER_SOURCES:.c=.%.o) lib/libfat.%.a lib/libbase.%.a
	${$*_executable}

.PHONY: clean_writer
clean_writer:
	rm -f $(WRITER_SOURCES:.c=.host.o)
	rm -f tool/writer.host.out

	rm -f $(WRITER_SOURCES:.c=.target.o)
	rm -f tool/writer.target.out
