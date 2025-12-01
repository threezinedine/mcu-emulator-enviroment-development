#pragma once
#if MD_DEBUG
#include <assert.h>
#endif

#define MD_PATH_SEPARATOR	  '/'
#define MD_PATH_SEPARATOR_STR "/"

#if PLATFORM_IS_WINDOWS
#define debugBreak() __debugbreak()
#else
#include <signal.h>
#define debugBreak() __builtin_trap()
#endif

#if PLATFORM_IS_WEB
#include <emscripten/emscripten.h>
#endif

#if MD_DEBUG
#define MD_ASSERT(expr)                                                                                                \
	do                                                                                                                 \
	{                                                                                                                  \
		if (!(expr))                                                                                                   \
		{                                                                                                              \
			struct MdConsoleConfig config;                                                                             \
			config.color = MD_CONSOLE_COLOR_RED;                                                                       \
			mdSetConsoleConfig(config);                                                                                \
			mdFormatPrint("Assertion failed: %s at %s:%d\n", #expr, __FILE__, __LINE__);                               \
			config.color = MD_CONSOLE_COLOR_RESET;                                                                     \
			mdSetConsoleConfig(config);                                                                                \
			debugBreak();                                                                                              \
		}                                                                                                              \
	} while (0)

#define MD_ASSERT_MSG(expr, msg, ...)                                                                                  \
	do                                                                                                                 \
	{                                                                                                                  \
		if (!(expr))                                                                                                   \
		{                                                                                                              \
			struct MdConsoleConfig config;                                                                             \
			config.color = MD_CONSOLE_COLOR_RED;                                                                       \
			mdSetConsoleConfig(config);                                                                                \
			char buffer[512];                                                                                          \
			mdFormatString(buffer, sizeof(buffer), msg, ##__VA_ARGS__);                                                \
			mdFormatPrint("Assertion failed: %s at %s:%d\n", buffer, __FILE__, __LINE__);                              \
			config.color = MD_CONSOLE_COLOR_RESET;                                                                     \
			mdSetConsoleConfig(config);                                                                                \
			debugBreak();                                                                                              \
		}                                                                                                              \
	} while (0)

// Macro to mark a variable as untouchable for debugging purposes.
#define MD_UNTOUCHABLE() MD_ASSERT_MSG(MD_FALSE, "This code path should be unreachable!")

#else
#define MD_ASSERT(expr)
#define MD_ASSERT_MSG(expr, msg, ...)
#define MD_UNTOUCHABLE()
#endif

/**
 * Macro to mark a variable as unused to avoid compiler warnings.
 */
#define MEED_UNUSED(x) (void)(x)

/**
 * This macro is used for marking that the symbol is intended for binding generation (python)
 */
#if defined(__clang__)
#define MD_BINDING __attribute__((annotate("binding")))
#define MD_HIDDEN  __attribute__((annotate("hidden")))
#else
#define MD_BINDING
#define MD_HIDDEN
#endif

#if defined(__EMSCRIPTEN__)
#define PLATFORM_API __attribute__((visibility("default"))) __attribute__((used))
#else
#define PLATFORM_API
#endif

// =================== Types defined ===================

typedef MD_BINDING unsigned char	  u8;
typedef MD_BINDING unsigned short	  u16;
typedef MD_BINDING unsigned int		  u32;
typedef MD_BINDING unsigned long long u64;

#if MD_DEBUG
static_assert(sizeof(u8) == 1, "u8 size is not 1 byte");
static_assert(sizeof(u16) == 2, "u16 size is not 2 bytes");
static_assert(sizeof(u32) == 4, "u32 size is not 4 bytes");
static_assert(sizeof(u64) == 8, "u64 size is not 8 bytes");
#endif

typedef MD_BINDING char		 i8;
typedef MD_BINDING short	 i16;
typedef MD_BINDING int		 i32;
typedef MD_BINDING long long i64;

#if MD_DEBUG
static_assert(sizeof(i8) == 1, "i8 size is not 1 byte");
static_assert(sizeof(i16) == 2, "i16 size is not 2 bytes");
static_assert(sizeof(i32) == 4, "i32 size is not 4 bytes");
static_assert(sizeof(i64) == 8, "i64 size is not 8 bytes");
#endif

typedef MD_BINDING float  f32;
typedef MD_BINDING double f64;

#if MD_DEBUG
static_assert(sizeof(f32) == 4, "f32 size is not 4 bytes");
static_assert(sizeof(f64) == 8, "f64 size is not 8 bytes");
#endif

typedef MD_BINDING unsigned char b8;
typedef MD_BINDING unsigned int	 b32;

#if MD_DEBUG
static_assert(sizeof(b8) == 1, "b8 size is not 1 byte");
static_assert(sizeof(b32) == 4, "b32 size is not 4 bytes");
#endif

typedef MD_BINDING size_t mdSize;

#define MD_NULL ((void*)0)

#define MD_TRUE	 ((b8)1)
#define MD_FALSE ((b8)0)

#define MD_ARRAY_SIZE(arr) (sizeof(arr) / sizeof((arr)[0]))

/**
 * Convert predefined macro to c string.
 *
 * @example
 * ```c
 * MD_STRINGIFY(PROJECT_BASE_DIR) -> "/path/to/project"
 * ```
 */
#define MD_STRINGIFY(x)	 _MD_STRINGIFY(x)
#define _MD_STRINGIFY(x) #x

#include "console.h"
#include "exceptions.h"
#include "memory.h"