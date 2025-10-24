// SPDX-Licence-Identifier: X11
// Copyright (C) 2025 Emily "TTG" Banerjee <prs.ttg+kml@pm.me>

#pragma once

#include <kml/base/result.h>
#include <kml/base/environment.h>
#include <kml/base/type.h>

/**
 * Per-character callback for string formatting using kml_base_string_format_callback_variadic().
 * \param[in] 0 The next formatted character.
 * \param[in,out] 1 The passthrough parameter given to kml_base_string_format_callback_variadic().
 * \return Any callee-specified error result.
 * \return KML_BASE_RESULT_OK on success.
 */
typedef enum kml_base_result kml_base_string_format_callback_t(int, void*);

/**
 * \see kml_base_string_format_buffer_length_variadic()
 * \return See kml_base_string_format_callback_variadic().
 */
enum kml_base_result kml_base_string_format_buffer_length(
		kml_base_size_t*, const char*, ...);

/**
 * \see kml_base_string_format_buffer_variadic()
 * \return See kml_base_string_format_callback_variadic().
 */
enum kml_base_result kml_base_string_format_buffer(char*, const char*, ...);

/**
 * Calculates the required size for an output buffer in a call to
 * kml_base_string_format_buffer_variadic() with the same format specifier
 * string and format parameters.
 * \param[out] 0 A pointer to output storage for the calculated size.
 * \param[in] 1 A format specifier string.
 * \param[in] 2 A variadic list with format parameters.
 * \return See kml_base_string_format_callback_variadic().
 * \see kml_base_string_format_callback_variadic() for format specification.
 */
enum kml_base_result kml_base_string_format_buffer_length_variadic(
		kml_base_size_t*, const char*, kml_base_variadic_t);

/**
 * Adapts kml_base_string_format_callback_variadic() to output to a user-allocated
 * character buffer.
 * \param[out] 0 A pointer to output storage for the formatted string.
 * \param[in] 1 A format specifier string.
 * \param[in] 2 A variadic list with format parameters.
 * \return See kml_base_string_format_callback_variadic().
 * \see kml_base_string_format_callback_variadic() for format specification.
 */
enum kml_base_result kml_base_string_format_buffer_variadic(
		char*, const char*, kml_base_variadic_t);

/**
 * Processes a format specifier string and provides substitutions to transform
 * format parameters into character output. The caller is responsible for handling
 * output data via. the provided callback function.
 *
 * The format specifier string will be copied character-wise until a `$` character
 * is encountered. The following character determines the type to consume the next
 * format parameter as:
 * | Format specifier character | Consumed type                             |
 * | -------------------------- | ----------------------------------------- |
 * | `I`                        | `int`                                     |
 * | `U`                        | `unsigned`                                |
 * | `R`                        | `enum kml_base_result`                    |
 * | `S`                        | `const char*`                             |
 * | `C`                        | `char`                                    |
 * | `P`                        | Any pointer type, or `kml_base_pointer_t` |
 * | `X`                        | `unsigned`                                |
 * \param[in] 0 A callback function to be called for each formatted character output.
 * \param[in,out] 1 An arbitrary pointer to be passed through to the callback function.
 * \param[in] 2 A format specifier string.
 * \param[in] 3 A variadic list with format parameters.
 * \return KML_BASE_RESULT_OK on success.
 * \return Any error result from the kml_base_string_format_callback_t.
 * \return KML_BASE_RESULT_ERROR_INVALID_FORMAT_SPECIFIER if an invalid format specifier is encountered.
 */
enum kml_base_result kml_base_string_format_callback_variadic(
		kml_base_string_format_callback_t, void*, const char*,
		kml_base_variadic_t);
