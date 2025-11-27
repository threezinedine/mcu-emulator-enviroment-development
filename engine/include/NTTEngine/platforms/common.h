#pragma once
#include <assert.h>

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef unsigned long long u64;

static_assert(sizeof(u8) == 1, "u8 size is not 1 byte");
static_assert(sizeof(u16) == 2, "u16 size is not 2 bytes");
static_assert(sizeof(u32) == 4, "u32 size is not 4 bytes");
static_assert(sizeof(u64) == 8, "u64 size is not 8 bytes");

typedef char i8;
typedef short i16;
typedef int i32;
typedef long long i64;

static_assert(sizeof(i8) == 1, "i8 size is not 1 byte");
static_assert(sizeof(i16) == 2, "i16 size is not 2 bytes");
static_assert(sizeof(i32) == 4, "i32 size is not 4 bytes");
static_assert(sizeof(i64) == 8, "i64 size is not 8 bytes");

typedef float f32;
typedef double f64;

static_assert(sizeof(f32) == 4, "f32 size is not 4 bytes");
static_assert(sizeof(f64) == 8, "f64 size is not 8 bytes");

typedef unsigned char b8;
typedef unsigned int b32;

static_assert(sizeof(b8) == 1, "b8 size is not 1 byte");
static_assert(sizeof(b32) == 4, "b32 size is not 4 bytes");

#define NTT_TRUE (b8)(1)
#define NTT_FALSE (b8)(0)

#define NTT_ARRAY_SIZE(arr) (sizeof(arr) / sizeof((arr)[0]))

#if PLATFORM_IS_WINDOWS
#define NTT_PATH_SEPARATOR '\\'
#define NTT_PATH_SEPARATOR_STR "\\"

#define debugBreak() __debugbreak()

#else

#define NTT_PATH_SEPARATOR '/'
#define NTT_PATH_SEPARATOR_STR "/"

#include <signal.h>
#define debugBreak() __builtin_trap()
#endif

#if NTT_DEBUG
#define NTT_ASSERT(expr)                                                       \
	do                                                                         \
	{                                                                          \
		if (!(expr))                                                           \
		{                                                                      \
			debugBreak();                                                      \
		}                                                                      \
	} while (0)

#define NTT_ASSERT_MSG(expr, msg)                                              \
	do                                                                         \
	{                                                                          \
		if (!(expr))                                                           \
		{                                                                      \
			nttPlatformSetColor(NTT_PLATFORM_COLOR_RED);                       \
			printf(                                                            \
				"Assertion failed: %s at %s:%d\n", msg, __FILE__, __LINE__);   \
			nttPlatformSetColor(NTT_PLATFORM_COLOR_RESET);                     \
			debugBreak();                                                      \
		}                                                                      \
	} while (0)
#else
#define NTT_ASSERT(expr)
#define NTT_ASSERT_MSG(expr, msg)
#endif

#if defined(__clang__)
#define NTT_BINDING __attribute__((attribute("binding")))
#else
#define NTT_BINDING
#endif