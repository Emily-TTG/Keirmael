// SPDX-Licence-Identifier: X11
// Copyright (C) 2025 Emily "TTG" Banerjee <prs.ttg+kml@pm.me>

#pragma once

#define KML_BASE_ASM __asm__ volatile

#ifdef _CLANGD
# define KML_BASE_LINTER
#endif

#ifdef __has_builtin
// NOTE: According to https://clang.llvm.org/docs/LanguageExtensions.html#has-builtin
//		 `__has_builtin` only works for builtin functions, pseudo functions or templates.
//# if __has_builtin(__builtin_va_list) || defined(KML_BASE_LINTER)
typedef __builtin_va_list kml_base_variadic_t;
//# endif
# if __has_builtin(__builtin_c23_va_start) || defined(KML_BASE_LINTER)
#  define KML_BASE_VARIADIC_START(variadic) __builtin_c23_va_start(variadic)
# endif
# if __has_builtin(__builtin_va_end) || defined(KML_BASE_LINTER)
#  define KML_BASE_VARIADIC_END(variadic) __builtin_va_end(variadic)
# endif
# if __has_builtin(__builtin_va_arg) || defined(KML_BASE_LINTER)
#  define KML_BASE_VARIADIC_GET(variadic, type) \
		__builtin_va_arg(variadic, type)
# endif

# if __has_builtin(__builtin_unreachable)
#  define KML_BASE_UNREACHABLE __builtin_unreachable
# endif
#endif

#ifdef __amd64__
# define KML_BASE_ARCH_64
#endif

#define KML_BASE_NULL ((void*) 0)
