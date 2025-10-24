// SPDX-Licence-Identifier: X11
// Copyright (C) 2025 Emily "TTG" Banerjee <prs.ttg+kml@pm.me>

#include <kml/base/environment.h>
#include <kml/base/type.h>
#include <kml/base/string.h>

enum kml_base_result kml_base_string_format_buffer(
		char* out, const char* format, ...) {

	kml_base_variadic_t variadic = {};
	KML_BASE_VARIADIC_START(variadic);

	const enum kml_base_result result = kml_base_string_format_buffer_variadic(
			out, format, variadic);

	KML_BASE_VARIADIC_END(variadic);
	return result;
}

struct kml_base_string_format_variadic_callback_output {
	char* buffer;
	kml_base_size_t offset;
};

static enum kml_base_result kml_base_string_format_buffer_callback(
		int character, void* passthrough) {

	const struct kml_base_string_format_variadic_callback_output* output =
			passthrough;

	output->buffer[output->offset] = (char) character;

	return KML_BASE_RESULT_OK;
}

enum kml_base_result kml_base_string_format_buffer_variadic(
		char* out, const char* format, kml_base_variadic_t variadic) {

	struct kml_base_string_format_variadic_callback_output output = {};
	output.buffer = out;

	return kml_base_string_format_callback_variadic(
			kml_base_string_format_buffer_callback, &output, format, variadic);
}

enum kml_base_result kml_base_string_format_callback_variadic(
		kml_base_string_format_callback_t callback, void* passthrough,
		const char* format, kml_base_variadic_t variadic) {

	enum kml_base_result result;

	for(; *format; ++format) {
		char character = *format;

		if(character != '$') {
			result = callback(character, passthrough);
			if(result) return result;

			continue;
		}

		character = *++format;

		kml_base_s64_t signed_value;
		kml_base_u64_t value;
		const char* string;

		int intermediate[20] = {};
		kml_base_size_t digit = 0;

		switch(character) {
			case 'C': {
				char format_character = KML_BASE_VARIADIC_GET(variadic, int);

				result = callback(format_character, passthrough);
				if(result) return result;

				break;
			}

			case 'I': {
				signed_value = KML_BASE_VARIADIC_GET(variadic, int);
				goto format_signed;
			}

			case 'U': {
				value = KML_BASE_VARIADIC_GET(variadic, int);
				goto format_unsigned;
			}

			case 'R': {
				string = kml_base_result_name(
						KML_BASE_VARIADIC_GET(variadic, enum kml_base_result));

				goto format_string;
			}

			case 'S': {
				string = KML_BASE_VARIADIC_GET(variadic, const char*);
				goto format_string;
			}

			case 'P': {
				value = KML_BASE_VARIADIC_GET(variadic, kml_base_pointer_t);
				goto format_hex;
			}

			case 'X': {
				value = KML_BASE_VARIADIC_GET(variadic, unsigned);
				goto format_hex;
			}

			default: return KML_BASE_RESULT_ERROR_INVALID_FORMAT_SPECIFIER;
		}

		continue;

	format_signed:
		if(signed_value < 0) {
			result = callback('-', passthrough);
			if(result) return result;
		}

		value = signed_value < 0 ? -signed_value : signed_value;

	format_unsigned:
		do {
			intermediate[digit++] = '0' + (int) (value % 10);
		} while(value /= 10);

		digit--;
		do {
			result = callback(intermediate[digit], passthrough);
			if(result) return result;
		} while(--digit > 0);

		continue;

	format_string:
		for(; *string; ++string) {
			result = callback(*string, passthrough);
			if(result) return result;
		}

		continue;

	format_hex:
		result = callback('0', passthrough);
		if(result) return result;
		result = callback('x', passthrough);
		if(result) return result;

		do {
			intermediate[digit++] = (int) "0123456789ABCDEF"[value % 16];
		} while(value /= 16);

		digit--;
		do {
			result = callback(intermediate[digit], passthrough);
			if(result) return result;
		} while(--digit > 0);
	}

	return KML_BASE_RESULT_OK;
}
