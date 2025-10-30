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
		case KML_BASE_RESULT_ERROR_INVALID_PARAMETER: return "invalid parameter value";
	}

	return "<INVALID RESULT VALUE>";
}
