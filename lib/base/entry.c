// SPDX-Licence-Identifier: X11
// Copyright (C) 2025 Emily "TTG" Banerjee <prs.ttg+kml@pm.me>

#include <kml/base/result.h>
#include <kml/base/log.h>

enum kml_base_result kml_base_main(void);

#ifdef KML_HOST
int main(void) {
	enum kml_base_result result = kml_base_main();
	if(result) [[clang::unlikely]] {
		kml_base_log_result(__FILE__, result, "kml_base_main()");
		return 1;
	}

	return 0;
}
#endif
