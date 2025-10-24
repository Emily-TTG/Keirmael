// SPDX-Licence-Identifier: X11
// Copyright (C) 2025 Emily "TTG" Banerjee <prs.ttg+kml@pm.me>

#pragma once

enum [[clang::enum_extensibility(closed)]] [[nodiscard]] kml_base_result {
	/// No error occurred.
	KML_BASE_RESULT_OK,
	/// An unknown error occurred.
	KML_BASE_RESULT_ERROR_UNKNOWN,
	/// An invalid format specifier was encountered.
	KML_BASE_RESULT_ERROR_INVALID_FORMAT_SPECIFIER,
	/// A system function call resulted in the setting of `errno`. This should
	/// be logged with the more descriptive system-specific interfaces in addition
	/// to the return of this result at the error site.
	KML_BASE_RESULT_ERROR_ERRNO,
	/// An invalid control flow path was reached.
	KML_BASE_RESULT_ERROR_INVALID_CONTROL_PATH,
	/// A parameter's value was out of expected range or format.
	KML_BASE_RESULT_ERROR_INVALID_PARAMETER
};

const char* kml_base_result_name(enum kml_base_result);
