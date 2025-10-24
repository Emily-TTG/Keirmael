// SPDX-Licence-Identifier: X11
// Copyright (C) 2025 Emily "TTG" Banerjee <prs.ttg+kml@pm.me>

#include <kml/base/result.h>

enum kml_base_result kml_base_main(void);

#ifdef KML_HOST
int main(void) {
	(void) kml_base_main();
	// TODO: Handle root result.
}
#endif
