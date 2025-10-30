// SPDX-Licence-Identifier: X11
// Copyright (C) 2025 Emily "TTG" Banerjee <prs.ttg+kml@pm.me>

#include <kml/base/log.h>
#include <kml/base/string.h>
#include <kml/base/environment.h>

static enum kml_base_result kml_base_log_format_callback(
		int character, void* passthrough) {

	(void) passthrough;

	return kml_base_log_character(character);
}

void kml_base_log_raw(const char* format, ...) {
	kml_base_variadic_t variadic = {};
	KML_BASE_VARIADIC_START(variadic);

	kml_base_log_raw_variadic(format, variadic);

	KML_BASE_VARIADIC_END(variadic);
}

void kml_base_log_raw_variadic(
		const char* format, kml_base_variadic_t variadic) {

	// TODO: Gracefully ignore error.
	(void) kml_base_string_format_callback_variadic(
			kml_base_log_format_callback, KML_BASE_NULL, format,
			variadic);
}

void kml_base_log(const char* file, const char* format, ...) {
	kml_base_variadic_t variadic = {};
	KML_BASE_VARIADIC_START(variadic);

	kml_base_log_variadic(file, format, variadic);

	KML_BASE_VARIADIC_END(variadic);
}

void kml_base_log_variadic(
		const char* file, const char* format, kml_base_variadic_t variadic) {

	kml_base_log_raw("[$S] ", file);
	kml_base_log_raw_variadic(format, variadic);
}

void kml_base_log_result(
		const char* file, enum kml_base_result result,
		const char* format, ...) {

	kml_base_variadic_t variadic = {};
	KML_BASE_VARIADIC_START(variadic);

	kml_base_log(file, "");
	kml_base_log_raw_variadic(format, variadic);
	kml_base_log_raw(": $R\n", result);

	KML_BASE_VARIADIC_END(variadic);
}
