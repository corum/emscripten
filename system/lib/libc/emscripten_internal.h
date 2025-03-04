/*
 * Copyright 2023 The Emscripten Authors.  All rights reserved.
 * Emscripten is available under two separate licenses, the MIT license and the
 * University of Illinois/NCSA Open Source License.  Both these licenses can be
 * found in the LICENSE file.
 *
 * Declarations for internal-only JS library functions.
 *
 * All JS library functions must be declares in one header or anther in order
 * for `tools/gen_sig_info.py` to work.   This file contains declarations for
 * functions that are not declared in any other public or private header.
 */

#include <emscripten/em_macros.h>

#include <signal.h>    // for `sighandler_t`
#include <stdbool.h>   // for `bool`
#include <stdint.h>    // for `intptr_t`
#include <sys/types.h> // for `off_t`
#include <time.h>      // for `struct tm`

#ifdef __cplusplus
extern "C" {
#endif

// An external JS implementation that is efficient for very large copies, using
// HEAPU8.set()
void emscripten_memcpy_big(void* __restrict__ dest,
                           const void* __restrict__ src,
                           size_t n) EM_IMPORT(emscripten_memcpy_big);

void emscripten_notify_memory_growth(size_t memory_index);

// Declare these functions `int` rather than time_t to avoid int64 at the wasm
// boundary (avoids 64-bit complexity at the boundary when WASM_BIGINT is
// missing).
// TODO(sbc): Covert back to `time_t` before 2038 ...
int _timegm_js(struct tm* tm);
int _mktime_js(struct tm* tm);
void _localtime_js(const time_t* __restrict__ t, struct tm* __restrict__ tm);
void _gmtime_js(const time_t* __restrict__ t, struct tm* __restrict__ tm);

void _tzset_js(long* timezone, int* daylight, char** tzname);

const char* emscripten_pc_get_function(uintptr_t pc);
const char* emscripten_pc_get_file(uintptr_t pc);
int emscripten_pc_get_line(uintptr_t pc);
int emscripten_pc_get_column(uintptr_t pc);

char* emscripten_get_module_name(char* buf, size_t length);
void* emscripten_builtin_mmap(
  void* addr, size_t length, int prot, int flags, int fd, off_t offset);
int emscripten_builtin_munmap(void* addr, size_t length);

uintptr_t emscripten_stack_snapshot(void);
uint32_t
emscripten_stack_unwind_buffer(uintptr_t pc, uintptr_t* buffer, uint32_t depth);

bool _emscripten_get_now_is_monotonic(void);

void _emscripten_get_progname(char*, int);

// Not defined in musl, but defined in library.js.  Included here to for
// the benefit of gen_sig_info.py
char* strptime_l(const char* __restrict __s,
                 const char* __restrict __fmt,
                 struct tm* __tp,
                 locale_t __loc);

int _mmap_js(size_t length,
             int prot,
             int flags,
             int fd,
             size_t offset,
             int* allocated,
             void** addr);
int _munmap_js(
  intptr_t addr, size_t length, int prot, int flags, int fd, size_t offset);
int _msync_js(
  intptr_t addr, size_t length, int prot, int flags, int fd, size_t offset);

struct dso;

typedef void (*dlopen_callback_func)(struct dso*, void* user_data);

void* _dlopen_js(struct dso* handle);
void* _dlsym_js(struct dso* handle, const char* symbol, int* sym_index);
void _emscripten_dlopen_js(struct dso* handle,
                           dlopen_callback_func onsuccess,
                           dlopen_callback_func onerror,
                           void* user_data);
void* _dlsym_catchup_js(struct dso* handle, int sym_index);

int _setitimer_js(int which, double timeout);

#ifdef _GNU_SOURCE
void __call_sighandler(sighandler_t handler, int sig);
#endif

double emscripten_get_now_res(void);

void* emscripten_return_address(int level);

void _emscripten_fs_load_embedded_files(void* ptr);

void _emscripten_throw_longjmp(void);

void __handle_stack_overflow(void* addr);

#ifdef __cplusplus
}
#endif
