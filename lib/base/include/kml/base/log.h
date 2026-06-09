// SPDX-Licence-Identifier: X11
// Copyright (C) 2025 Emily "TTG" Banerjee <prs.ttg+kml@pm.me>

#pragma once

#include <kml/base/result.h>
#include <kml/base/environment.h>

enum kml_base_result kml_base_log_character(int);

void kml_base_log_raw(const char*, ...);
void kml_base_log_raw_variadic(const char*, kml_base_variadic_t);
void kml_base_log(const char*, const char*, ...);
void kml_base_log_variadic(const char*, const char*, kml_base_variadic_t);
void kml_base_log_result(const char*, enum kml_base_result, const char*, ...);

#define KML_BASE_LOG_RAW(...) kml_base_log_raw (__FILE__, __VA_ARGS__)
#define KML_BASE_LOG_RAW_VARIADIC(...) kml_base_log_raw_variadic (__FILE__, __VA_ARGS__)
#define KML_BASE_LOG(...) kml_base_log (__FILE__, __VA_ARGS__)
#define KML_BASE_LOG_VARIADIC(...) kml_base_log_variadic (__FILE__, __VA_ARGS__)
#define KML_BASE_LOG_RESULT(...) kml_base_log_result (__FILE__, __VA_ARGS__)
