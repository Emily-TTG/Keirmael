// SPDX-Licence-Identifier: X11
// Copyright (C) 2025 Emily "TTG" Banerjee <prs.ttg+kml@pm.me>

#include <kml/base/result.h>

const char* kml_base_result_name(enum kml_base_result result) {
	switch(result) {
		case KML_BASE_RESULT_OK: return "ok";
		case KML_BASE_RESULT_ERROR_UNKNOWN: return "unknown error";
		case KML_BASE_RESULT_ERROR_INVALID_FORMAT_SPECIFIER: return "invalid format specifier";
		case KML_BASE_RESULT_ERROR_INVALID_CONTROL_PATH: return "invalid control path";
		case KML_BASE_RESULT_ERROR_ERRNO: return "errno";
		case KML_BASE_RESULT_ERROR_INVALID_PARAMETER: return "invalid parameter";
		case KML_BASE_RESULT_ERROR_OUT_OF_MEMORY: return "out of memory";
		case KML_BASE_RESULT_ERROR_BUFFER_TOO_SMALL: return "buffer too small";
		case KML_BASE_RESULT_ERROR_INTEGRITY_TEST_FAILED: return "integrity test failed";
		case KML_BASE_RESULT_ERROR_UNIT_TEST_FAILED: return "unit test failed";
		case KML_BASE_RESULT_ERROR_OUT_OF_RANGE: return "out of range";
		case KML_BASE_RESULT_ERROR_NOT_IMPLEMENTED: return "not implemented";
		case KML_BASE_RESULT_ERROR_PARAMETER_LIFETIME_OVER: return "out of lifetime";
		case KML_BASE_RESULT_ERROR_VALUE_ALREADY_SET: return "value already set";
		case KML_BASE_RESULT_ERROR_PARAMETER_NOT_ALIGNED: return "not aligned";
	}

	return "<INVALID RESULT VALUE>";
}
