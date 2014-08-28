/*
 * Copyright (c) 2011 The Native Client Authors. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */
/*
 * all macros, definitions and small functions which
 * not fit to other header and often used are here
 *
 * Copyright (c) 2012, LiteStack, Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef TOOLS_H_
#define TOOLS_H_

#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <glib.h>
#include <sys/stat.h>

/*
 * putting extern "C" { } in header files make emacs want to indent
 * everything, which looks odd.  rather than putting in fancy syntax
 * recognition in c-mode, we just use the following macros.
 */
#ifdef __cplusplus
# define EXTERN_C_BEGIN extern "C" {
# define EXTERN_C_END }

/* Mark this function as not throwing beyond */
#else
# define EXTERN_C_BEGIN
# define EXTERN_C_END
#endif

/* gcc only */
#define INLINE __inline__
#define NORETURN __attribute__((noreturn))

/* generic constants */
#define BIG_ENOUGH_STRING 1024
#define MICROS_PER_MILLI 1000
#define MICRO_PER_SEC 1000000

/* utilities */
#define TOPTR(a) ((void*)(uintptr_t)(a))
#define TOINT(a) ((void*)(uintptr_t)(a))
#define ROUNDDOWN_64K(a) ((a) & ~(NACL_MAP_PAGESIZE - 1LLU))
#define ROUNDUP_64K(a) ROUNDDOWN_64K((a) + NACL_MAP_PAGESIZE - 1LLU)
#define ARRAY_SIZE(arr) ((sizeof arr)/sizeof arr[0])
#define ARRAY_SIZE_SAFE(arr) (NaClArrayCheckHelper( \
    __builtin_types_compatible_p(__typeof__(&arr[0]), \
    __typeof__(arr))), ARRAY_SIZE(arr))

/*
 * Doing runtime checks is not really necessary -- this code is in
 * fact unreachable code that gets optimized out when used with the
 * ARRAY_SIZE_SAFE definition below.
 *
 * The runtime check is only useful when the build system is using
 * the inappropriate flags (e.g., missing -pedantic -Werror or
 * -pedantic-error), in which case instead of a compile-time error,
 * we'd get a runtime error.
 */
static INLINE void *NaClArrayCheckHelper(void *arg)
{
  if(NULL != arg) abort();
  return arg;
}

/*
 * ASSERT_SAME_SIZE(t1, t2) verifies that the two types have the same size
 * (as reported by sizeof).  When the check fails it generates a somewhat
 * opaque warning, mitigated by the variable's name.
 *
 * Examples:
 *   ASSERT_SAME_SIZE(void*, char*); // Likely to succeed!
 *   ASSERT_SAME_SIZE(char, long); // Unlikely to succeed
 */
#define ASSERT_SAME_SIZE(t1, t2) \
  do {char tested_types_are_not_the_same_size[sizeof(t1) == sizeof(t2)]; \
      (void) tested_types_are_not_the_same_size;} while (0)

#endif /* TOOLS_H_ */
