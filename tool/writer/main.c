// SPDX-Licence-Identifier: X11
// Copyright (C) 2025 Emily "TTG" Banerjee <prs.ttg+kml@pm.me>

#include <kml/base/result.h>
#include <kml/base/log.h>

enum kml_base_result kml_base_main(void) {
	kml_base_log_result(__FILE__, KML_BASE_RESULT_ERROR_UNKNOWN, "foo($I)", 3);
	return KML_BASE_RESULT_OK;
}
