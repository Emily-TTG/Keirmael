// SPDX-Licence-Identifier: X11
// Copyright (C) 2025 Emily "TTG" Banerjee <prs.ttg+kml@pm.me>

#include <kml/base/log.h>
#include <kml/base/host/host.h>

#include <stdio.h>
#include <string.h>
#include <errno.h>

enum kml_base_result kml_base_log_character(int character) {
	int result = putchar(character);
	if(result < 0) perror("putchar");

	return KML_BASE_RESULT_OK;
}

void kml_base_host_log_errno(const char* file, const char* format, ...) {
	kml_base_variadic_t variadic = {};
	KML_BASE_VARIADIC_START(variadic);

	kml_base_log_variadic(file, format, variadic);
	kml_base_log_raw(": $S", strerror(errno));

	KML_BASE_VARIADIC_END(variadic);
}
