/*
 * Copyright (c) 2016      Mellanox Technologies, Inc.
 *                         All rights reserved.
 * $COPYRIGHT$
 *
 * Additional copyrights may follow
 *
 * $HEADER$
 */

#include "ibprof_cmn.h"
#include "ibprof_api.h"
#include "ibprof_types.h"

#if defined(USE_SHMEM) && (USE_SHMEM == 1)

#include <stdbool.h>
#include <shmem.h>

#include "ibprof_shmem.h"

/* Check SHMEM API version */
#if defined(SHMEM_MAJOR_VERSION) && (SHMEM_MAJOR_VERSION == 1) && defined(SHMEM_MINOR_VERSION) && (SHMEM_MINOR_VERSION >= 2)
#if defined(OSHMEM_MAJOR_VERSION) && (OSHMEM_MAJOR_VERSION < 2)
#error It is not compatible SHMEM API version
#endif
#else
#error SHMEM API version 1.3+ is supported
#endif

/* SHMEM API */
#define OP_ON_MEMBERS_LIST(OP) \
	OP(shmem_init) \
	OP(shmem_finalize) \
\
	OP(shmem_n_pes) \
	OP(shmem_my_pe) \
\
	OP(shmem_pe_accessible) \
	OP(shmem_addr_accessible) \
\
	OP(shmem_malloc) \
	OP(shmem_align) \
	OP(shmem_realloc) \
	OP(shmem_free) \
\
	OP(shmem_ptr) \
\
	OP(shmem_char_p) \
	OP(shmem_short_p) \
	OP(shmem_int_p) \
	OP(shmem_long_p) \
	OP(shmem_float_p) \
	OP(shmem_double_p) \
	OP(shmem_longlong_p) \
	OP(shmem_longdouble_p) \
\
	OP(shmem_short_put) \
	OP(shmem_int_put) \
	OP(shmem_long_put) \
	OP(shmem_float_put) \
	OP(shmem_double_put) \
	OP(shmem_longlong_put) \
	OP(shmem_longdouble_put) \
	OP(shmem_put32) \
	OP(shmem_put64) \
	OP(shmem_put128) \
	OP(shmem_putmem) \
\
	OP(shmem_int_iput) \
	OP(shmem_short_iput) \
	OP(shmem_float_iput) \
	OP(shmem_double_iput) \
	OP(shmem_longlong_iput) \
	OP(shmem_longdouble_iput) \
	OP(shmem_long_iput) \
	OP(shmem_iput32) \
	OP(shmem_iput64) \
	OP(shmem_iput128) \
\
	OP(shmem_char_g) \
	OP(shmem_short_g) \
	OP(shmem_int_g) \
	OP(shmem_long_g) \
	OP(shmem_float_g) \
	OP(shmem_double_g) \
	OP(shmem_longlong_g) \
	OP(shmem_longdouble_g) \
\
	OP(shmem_short_get) \
	OP(shmem_int_get) \
	OP(shmem_long_get) \
	OP(shmem_float_get) \
	OP(shmem_double_get) \
	OP(shmem_longlong_get) \
	OP(shmem_longdouble_get) \
	OP(shmem_get32) \
	OP(shmem_get64) \
	OP(shmem_get128) \
	OP(shmem_getmem) \
\
	OP(shmem_int_iget) \
	OP(shmem_short_iget) \
	OP(shmem_float_iget) \
	OP(shmem_double_iget) \
	OP(shmem_longlong_iget) \
	OP(shmem_longdouble_iget) \
	OP(shmem_long_iget) \
	OP(shmem_iget32) \
	OP(shmem_iget64) \
	OP(shmem_iget128) \
\
	OP(shmem_swap) \
	OP(shmem_double_swap) \
	OP(shmem_float_swap) \
	OP(shmem_int_swap) \
	OP(shmem_long_swap) \
	OP(shmem_longlong_swap) \
\
	OP(shmem_int_cswap) \
	OP(shmem_long_cswap) \
	OP(shmem_longlong_cswap) \
\
	OP(shmem_int_fadd) \
	OP(shmem_long_fadd) \
	OP(shmem_longlong_fadd) \
\
	OP(shmem_int_finc) \
	OP(shmem_long_finc) \
	OP(shmem_longlong_finc) \
\
	OP(shmem_int_add) \
	OP(shmem_long_add) \
	OP(shmem_longlong_add) \
\
	OP(shmem_int_inc) \
	OP(shmem_long_inc) \
	OP(shmem_longlong_inc) \
\
	OP(shmem_set_lock) \
	OP(shmem_clear_lock) \
	OP(shmem_test_lock) \
\
	OP(shmem_short_wait) \
	OP(shmem_int_wait) \
	OP(shmem_long_wait) \
	OP(shmem_longlong_wait) \
	OP(shmem_wait) \
	OP(shmem_short_wait_until) \
	OP(shmem_int_wait_until) \
	OP(shmem_long_wait_until) \
	OP(shmem_longlong_wait_until) \
	OP(shmem_wait_until) \
\
	OP(shmem_barrier) \
	OP(shmem_barrier_all) \
	OP(shmem_fence) \
	OP(shmem_quiet) \
\
	OP(shmem_broadcast32) \
	OP(shmem_broadcast64) \
	OP(shmem_broadcast) \
	OP(shmem_collect32) \
	OP(shmem_collect64) \
	OP(shmem_fcollect32) \
	OP(shmem_fcollect64) \
\
	OP(shmem_short_and_to_all) \
	OP(shmem_int_and_to_all) \
	OP(shmem_long_and_to_all) \
	OP(shmem_longlong_and_to_all) \
\
	OP(shmem_short_or_to_all) \
	OP(shmem_int_or_to_all) \
	OP(shmem_long_or_to_all) \
	OP(shmem_longlong_or_to_all) \
\
	OP(shmem_short_xor_to_all) \
	OP(shmem_int_xor_to_all) \
	OP(shmem_long_xor_to_all) \
	OP(shmem_longlong_xor_to_all) \
\
	OP(shmem_short_max_to_all) \
	OP(shmem_int_max_to_all) \
	OP(shmem_long_max_to_all) \
	OP(shmem_longlong_max_to_all) \
	OP(shmem_float_max_to_all) \
	OP(shmem_double_max_to_all) \
	OP(shmem_longdouble_max_to_all) \
\
	OP(shmem_short_min_to_all) \
	OP(shmem_int_min_to_all) \
	OP(shmem_long_min_to_all) \
	OP(shmem_longlong_min_to_all) \
	OP(shmem_float_min_to_all) \
	OP(shmem_double_min_to_all) \
	OP(shmem_longdouble_min_to_all) \
\
	OP(shmem_short_sum_to_all) \
	OP(shmem_int_sum_to_all) \
	OP(shmem_long_sum_to_all) \
	OP(shmem_longlong_sum_to_all) \
	OP(shmem_float_sum_to_all) \
	OP(shmem_double_sum_to_all) \
	OP(shmem_longdouble_sum_to_all) \
	OP(shmem_complexf_sum_to_all) \
	OP(shmem_complexd_sum_to_all) \
\
	OP(shmem_short_prod_to_all) \
	OP(shmem_int_prod_to_all) \
	OP(shmem_long_prod_to_all) \
	OP(shmem_longlong_prod_to_all) \
	OP(shmem_float_prod_to_all) \
	OP(shmem_double_prod_to_all) \
	OP(shmem_longdouble_prod_to_all) \
	OP(shmem_complexf_prod_to_all) \
	OP(shmem_complexd_prod_to_all) \
\
	OP(shmem_udcflush) \
	OP(shmem_udcflush_line) \
	OP(shmem_set_cache_inv) \
	OP(shmem_set_cache_line_inv) \
	OP(shmem_clear_cache_inv) \
	OP(shmem_clear_cache_line_inv)


#define OP_ON_MEMBERS_LIST_EX(OP) \
	OP(shmem_char_put_nbi) \
	OP(shmem_short_put_nbi) \
	OP(shmem_int_put_nbi) \
	OP(shmem_long_put_nbi) \
	OP(shmem_float_put_nbi) \
	OP(shmem_double_put_nbi) \
	OP(shmem_longlong_put_nbi) \
	OP(shmem_longdouble_put_nbi) \
	OP(shmem_put8_nbi) \
	OP(shmem_put16_nbi) \
	OP(shmem_put32_nbi) \
	OP(shmem_put64_nbi) \
	OP(shmem_put128_nbi) \
	OP(shmem_putmem_nbi) \
\
	OP(shmem_char_get_nbi) \
	OP(shmem_short_get_nbi) \
	OP(shmem_int_get_nbi) \
	OP(shmem_long_get_nbi) \
	OP(shmem_float_get_nbi) \
	OP(shmem_double_get_nbi) \
	OP(shmem_longlong_get_nbi) \
	OP(shmem_longdouble_get_nbi) \
	OP(shmem_get8_nbi) \
	OP(shmem_get16_nbi) \
	OP(shmem_get32_nbi) \
	OP(shmem_get64_nbi) \
	OP(shmem_get128_nbi) \
	OP(shmem_getmem_nbi) \
\
	OP(shmem_alltoall32) \
	OP(shmem_alltoall64) \
	OP(shmem_alltoalls32) \
	OP(shmem_alltoalls64) \


/* Declare SHMEM API functions to substitute original from (lib)shmem library */
OP_ON_MEMBERS_LIST(DECLARE_TYPE)

/* A structure to store our internal calls depending on a mode */
struct shmem_module_api_t {
	OP_ON_MEMBERS_LIST(DECLARE_STRUCT_MEMBER)
};

#define DECLARE_OPTION_STRUCT(TYPE) \
struct shmem_module_api_t shmem_##TYPE##_funcs = { \
	OP_ON_MEMBERS_LIST(PREFIX##_##TYPE) \
};

static struct module_context_t {
	struct shmem_module_api_t	noble;	/* real call */
	struct shmem_module_api_t	mean;	/* our call */
} shmem_module_context;


#define DEFAULT_SYMVER     NULL

#define check_dlsym(func)  check_dlsymv(func, DEFAULT_SYMVER)
#define check_dlsymv(_func, _ver)  \
	do {                                                                   \
		shmem_module_context.noble._func = sys_dlsym(#_func, _ver);     \
		if (!shmem_module_context.noble._func)                          \
			status = IBPROF_ERR_UNSUPPORTED;                       \
	} while (0)


#define DECLARE_OPTION_FUNCTIONS_PROTOTYPED(TYPE) \
	void TYPE ## shmem_init(void) \
		{ FUNC_BODY_VOID(TYPE, shmem_init) }; \
	void TYPE ## shmem_finalize(void) \
		{ FUNC_BODY_VOID(TYPE, shmem_finalize) }; \
\
	int TYPE ## shmem_n_pes(void) \
		{ FUNC_BODY_INT(TYPE, shmem_n_pes) }; \
	int TYPE ## shmem_my_pe(void) \
		{ FUNC_BODY_INT(TYPE, shmem_my_pe) }; \
\
	int TYPE ## shmem_pe_accessible(int pe) \
		{ FUNC_BODY_INT(TYPE, shmem_pe_accessible, pe) }; \
	int TYPE ## shmem_addr_accessible(const void *addr, int pe) \
		{ FUNC_BODY_INT(TYPE, shmem_addr_accessible, addr, pe) }; \
\
	void* TYPE ## shmem_malloc(size_t size) \
		{ FUNC_BODY_PTR(TYPE, shmem_malloc, size) }; \
	void* TYPE ## shmem_align(size_t align, size_t size) \
		{ FUNC_BODY_PTR(TYPE, shmem_align, align, size) }; \
	void* TYPE ## shmem_realloc(void *ptr, size_t size) \
		{ FUNC_BODY_PTR(TYPE, shmem_realloc, ptr, size) }; \
	void TYPE ## shmem_free(void *ptr) \
		{ FUNC_BODY_VOID(TYPE, shmem_free, ptr) }; \
\
	void* TYPE ## shmem_ptr(const void *ptr, int pe) \
		{ FUNC_BODY_PTR(TYPE, shmem_ptr, ptr, pe) }; \
\
	void TYPE ## shmem_char_p(char* addr, char value, int pe) \
		{ FUNC_BODY_VOID(TYPE, shmem_char_p, addr, value, pe) }; \
	void TYPE ## shmem_short_p(short* addr, short value, int pe) \
		{ FUNC_BODY_VOID(TYPE, shmem_short_p, addr, value, pe) }; \
	void TYPE ## shmem_int_p(int* addr, int value, int pe) \
		{ FUNC_BODY_VOID(TYPE, shmem_int_p, addr, value, pe) }; \
	void TYPE ## shmem_long_p(long* addr, long value, int pe) \
		{ FUNC_BODY_VOID(TYPE, shmem_long_p, addr, value, pe) }; \
	void TYPE ## shmem_float_p(float* addr, float value, int pe) \
		{ FUNC_BODY_VOID(TYPE, shmem_float_p, addr, value, pe) }; \
	void TYPE ## shmem_double_p(double* addr, double value, int pe) \
		{ FUNC_BODY_VOID(TYPE, shmem_double_p, addr, value, pe) }; \
	void TYPE ## shmem_longlong_p(long long* addr, long long value, int pe) \
		{ FUNC_BODY_VOID(TYPE, shmem_longlong_p, addr, value, pe) }; \
	void TYPE ## shmem_longdouble_p(long double* addr, long double value, int pe) \
		{ FUNC_BODY_VOID(TYPE, shmem_longdouble_p, addr, value, pe) }; \
\
	void TYPE ## shmem_short_put(short *target, const short *source, size_t len, int pe) \
		{ FUNC_BODY_VOID(TYPE, shmem_short_put, target, source, len, pe) }; \
	void TYPE ## shmem_int_put(int *target, const int *source, size_t len, int pe) \
		{ FUNC_BODY_VOID(TYPE, shmem_int_put, target, source, len, pe) }; \
	void TYPE ## shmem_long_put(long *target, const long *source, size_t len, int pe) \
		{ FUNC_BODY_VOID(TYPE, shmem_long_put, target, source, len, pe) }; \
	void TYPE ## shmem_float_put(float *target, const float *source, size_t len, int pe) \
		{ FUNC_BODY_VOID(TYPE, shmem_float_put, target, source, len, pe) }; \
	void TYPE ## shmem_double_put(double *target, const double *source, size_t len, int pe) \
		{ FUNC_BODY_VOID(TYPE, shmem_double_put, target, source, len, pe) }; \
	void TYPE ## shmem_longlong_put(long long *target, const long long *source, size_t len, int pe) \
		{ FUNC_BODY_VOID(TYPE, shmem_longlong_put, target, source, len, pe) }; \
	void TYPE ## shmem_longdouble_put(long double *target, const long double *source, size_t len, int pe) \
		{ FUNC_BODY_VOID(TYPE, shmem_longdouble_put, target, source, len, pe) }; \
	void TYPE ## shmem_put32(void *target, const void *source, size_t len, int pe) \
		{ FUNC_BODY_VOID(TYPE, shmem_put32, target, source, len, pe) }; \
	void TYPE ## shmem_put64(void *target, const void *source, size_t len, int pe) \
		{ FUNC_BODY_VOID(TYPE, shmem_put64, target, source, len, pe) }; \
	void TYPE ## shmem_put128(void *target, const void *source, size_t len, int pe) \
		{ FUNC_BODY_VOID(TYPE, shmem_put128, target, source, len, pe) }; \
	void TYPE ## shmem_putmem(void *target, const void *source, size_t len, int pe) \
		{ FUNC_BODY_VOID(TYPE, shmem_putmem, target, source, len, pe) }; \
\
	void TYPE ## shmem_short_iput(short *target, const short *source, ptrdiff_t tst, ptrdiff_t sst, size_t len, int pe) \
		{ FUNC_BODY_VOID(TYPE, shmem_short_iput, target, source, tst, sst, len, pe) }; \
	void TYPE ## shmem_int_iput(int *target, const int *source, ptrdiff_t tst, ptrdiff_t sst, size_t len, int pe) \
		{ FUNC_BODY_VOID(TYPE, shmem_int_iput, target, source, tst, sst, len, pe) }; \
	void TYPE ## shmem_long_iput(long *target, const long *source, ptrdiff_t tst, ptrdiff_t sst, size_t len, int pe) \
		{ FUNC_BODY_VOID(TYPE, shmem_long_iput, target, source, tst, sst, len, pe) }; \
	void TYPE ## shmem_float_iput(float *target, const float *source, ptrdiff_t tst, ptrdiff_t sst, size_t len, int pe) \
		{ FUNC_BODY_VOID(TYPE, shmem_float_iput, target, source, tst, sst, len, pe) }; \
	void TYPE ## shmem_double_iput(double *target, const double *source, ptrdiff_t tst, ptrdiff_t sst, size_t len, int pe) \
		{ FUNC_BODY_VOID(TYPE, shmem_double_iput, target, source, tst, sst, len, pe) }; \
	void TYPE ## shmem_longlong_iput(long long *target, const long long *source, ptrdiff_t tst, ptrdiff_t sst, size_t len, int pe) \
		{ FUNC_BODY_VOID(TYPE, shmem_longlong_iput, target, source, tst, sst, len, pe) }; \
	void TYPE ## shmem_longdouble_iput(long double *target, const long double *source, ptrdiff_t tst, ptrdiff_t sst, size_t len, int pe) \
		{ FUNC_BODY_VOID(TYPE, shmem_longdouble_iput, target, source, tst, sst, len, pe) }; \
	void TYPE ## shmem_iput32(void *target, const void *source, ptrdiff_t tst, ptrdiff_t sst, size_t len, int pe) \
		{ FUNC_BODY_VOID(TYPE, shmem_iput32, target, source, tst, sst, len, pe) }; \
	void TYPE ## shmem_iput64(void *target, const void *source, ptrdiff_t tst, ptrdiff_t sst, size_t len, int pe) \
		{ FUNC_BODY_VOID(TYPE, shmem_iput64, target, source, tst, sst, len, pe) }; \
	void TYPE ## shmem_iput128(void *target, const void *source, ptrdiff_t tst, ptrdiff_t sst, size_t len, int pe) \
		{ FUNC_BODY_VOID(TYPE, shmem_iput128, target, source, tst, sst, len, pe) }; \
\
	char TYPE ## shmem_char_g(const char* addr, int pe) \
		{ FUNC_BODY_ANY(char, TYPE, shmem_char_g, addr, pe) }; \
	short TYPE ## shmem_short_g(const short* addr, int pe) \
		{ FUNC_BODY_ANY(short, TYPE, shmem_short_g, addr, pe) }; \
	int TYPE ## shmem_int_g(const int* addr, int pe) \
		{ FUNC_BODY_ANY(int, TYPE, shmem_int_g, addr, pe) }; \
	long TYPE ## shmem_long_g(const long* addr, int pe) \
		{ FUNC_BODY_ANY(long, TYPE, shmem_long_g, addr, pe) }; \
	float TYPE ## shmem_float_g(const float* addr, int pe) \
		{ FUNC_BODY_ANY(float, TYPE, shmem_float_g, addr, pe) }; \
	double TYPE ## shmem_double_g(const double* addr, int pe) \
		{ FUNC_BODY_ANY(double, TYPE, shmem_double_g, addr, pe) }; \
	long long TYPE ## shmem_longlong_g(const long long* addr, int pe) \
		{ FUNC_BODY_ANY(long long, TYPE, shmem_longlong_g, addr, pe) }; \
	long double TYPE ## shmem_longdouble_g(const long double* addr, int pe) \
		{ FUNC_BODY_ANY(long double, TYPE, shmem_longdouble_g, addr, pe) }; \
\
	void TYPE ## shmem_short_get(short *target, const short *source, size_t len, int pe) \
		{ FUNC_BODY_VOID(TYPE, shmem_short_get, target, source, len, pe) }; \
	void TYPE ## shmem_int_get(int *target, const int *source, size_t len, int pe) \
		{ FUNC_BODY_VOID(TYPE, shmem_int_get, target, source, len, pe) }; \
	void TYPE ## shmem_long_get(long *target, const long *source, size_t len, int pe) \
		{ FUNC_BODY_VOID(TYPE, shmem_long_get, target, source, len, pe) }; \
	void TYPE ## shmem_float_get(float *target, const float *source, size_t len, int pe) \
		{ FUNC_BODY_VOID(TYPE, shmem_float_get, target, source, len, pe) }; \
	void TYPE ## shmem_double_get(double *target, const double *source, size_t len, int pe) \
		{ FUNC_BODY_VOID(TYPE, shmem_double_get, target, source, len, pe) }; \
	void TYPE ## shmem_longlong_get(long long *target, const long long *source, size_t len, int pe) \
		{ FUNC_BODY_VOID(TYPE, shmem_longlong_get, target, source, len, pe) }; \
	void TYPE ## shmem_longdouble_get(long double *target, const long double *source, size_t len, int pe) \
		{ FUNC_BODY_VOID(TYPE, shmem_longdouble_get, target, source, len, pe) }; \
	void TYPE ## shmem_get32(void *target, const void *source, size_t len, int pe) \
		{ FUNC_BODY_VOID(TYPE, shmem_get32, target, source, len, pe) }; \
	void TYPE ## shmem_get64(void *target, const void *source, size_t len, int pe) \
		{ FUNC_BODY_VOID(TYPE, shmem_get64, target, source, len, pe) }; \
	void TYPE ## shmem_get128(void *target, const void *source, size_t len, int pe) \
		{ FUNC_BODY_VOID(TYPE, shmem_get128, target, source, len, pe) }; \
	void TYPE ## shmem_getmem(void *target, const void *source, size_t len, int pe) \
		{ FUNC_BODY_VOID(TYPE, shmem_getmem, target, source, len, pe) }; \
\
	void TYPE ## shmem_short_iget(short *target, const short *source, ptrdiff_t tst, ptrdiff_t sst, size_t len, int pe) \
		{ FUNC_BODY_VOID(TYPE, shmem_short_iget, target, source, tst, sst, len, pe) }; \
	void TYPE ## shmem_int_iget(int *target, const int *source, ptrdiff_t tst, ptrdiff_t sst, size_t len, int pe) \
		{ FUNC_BODY_VOID(TYPE, shmem_int_iget, target, source, tst, sst, len, pe) }; \
	void TYPE ## shmem_long_iget(long *target, const long *source, ptrdiff_t tst, ptrdiff_t sst, size_t len, int pe) \
		{ FUNC_BODY_VOID(TYPE, shmem_long_iget, target, source, tst, sst, len, pe) }; \
	void TYPE ## shmem_float_iget(float *target, const float *source, ptrdiff_t tst, ptrdiff_t sst, size_t len, int pe) \
		{ FUNC_BODY_VOID(TYPE, shmem_float_iget, target, source, tst, sst, len, pe) }; \
	void TYPE ## shmem_double_iget(double *target, const double *source, ptrdiff_t tst, ptrdiff_t sst, size_t len, int pe) \
		{ FUNC_BODY_VOID(TYPE, shmem_double_iget, target, source, tst, sst, len, pe) }; \
	void TYPE ## shmem_longlong_iget(long long *target, const long long *source, ptrdiff_t tst, ptrdiff_t sst, size_t len, int pe) \
		{ FUNC_BODY_VOID(TYPE, shmem_longlong_iget, target, source, tst, sst, len, pe) }; \
	void TYPE ## shmem_longdouble_iget(long double *target, const long double *source, ptrdiff_t tst, ptrdiff_t sst, size_t len, int pe) \
		{ FUNC_BODY_VOID(TYPE, shmem_longdouble_iget, target, source, tst, sst, len, pe) }; \
	void TYPE ## shmem_iget32(void *target, const void *source, ptrdiff_t tst, ptrdiff_t sst, size_t len, int pe) \
		{ FUNC_BODY_VOID(TYPE, shmem_iget32, target, source, tst, sst, len, pe) }; \
	void TYPE ## shmem_iget64(void *target, const void *source, ptrdiff_t tst, ptrdiff_t sst, size_t len, int pe) \
		{ FUNC_BODY_VOID(TYPE, shmem_iget64, target, source, tst, sst, len, pe) }; \
	void TYPE ## shmem_iget128(void *target, const void *source, ptrdiff_t tst, ptrdiff_t sst, size_t len, int pe) \
		{ FUNC_BODY_VOID(TYPE, shmem_iget128, target, source, tst, sst, len, pe) }; \
\
	long TYPE ## shmem_swap(long *target, long value, int pe) \
		{ FUNC_BODY_ANY(long, TYPE, shmem_swap, target, value, pe) }; \
	double TYPE ## shmem_double_swap(double *target, double value, int pe) \
		{ FUNC_BODY_ANY(double, TYPE, shmem_double_swap, target, value, pe) }; \
	float TYPE ## shmem_float_swap(float *target, float value, int pe) \
		{ FUNC_BODY_ANY(float, TYPE, shmem_float_swap, target, value, pe) }; \
	int TYPE ## shmem_int_swap(int *target, int value, int pe) \
		{ FUNC_BODY_ANY(int, TYPE, shmem_int_swap, target, value, pe) }; \
	long TYPE ## shmem_long_swap(long *target, long value, int pe) \
		{ FUNC_BODY_ANY(long, TYPE, shmem_long_swap, target, value, pe) }; \
	long long TYPE ## shmem_longlong_swap(long long *target, long long value, int pe) \
		{ FUNC_BODY_ANY(long long, TYPE, shmem_longlong_swap, target, value, pe) }; \
\
	int TYPE ## shmem_int_cswap(int *target, int cond, int value, int pe) \
		{ FUNC_BODY_ANY(int, TYPE, shmem_int_cswap, target, cond, value, pe) }; \
	long TYPE ## shmem_long_cswap(long *target, long cond, long value, int pe) \
		{ FUNC_BODY_ANY(long, TYPE, shmem_long_cswap, target, cond, value, pe) }; \
	long long TYPE ## shmem_longlong_cswap(long long *target, long long cond, long long value, int pe) \
		{ FUNC_BODY_ANY(long long, TYPE, shmem_longlong_cswap, target, cond, value, pe) }; \
\
	int TYPE ## shmem_int_fadd(int *target, int value, int pe) \
		{ FUNC_BODY_ANY(int, TYPE, shmem_int_fadd, target, value, pe) }; \
	long TYPE ## shmem_long_fadd(long *target, long value, int pe) \
		{ FUNC_BODY_ANY(long, TYPE, shmem_long_fadd, target, value, pe) }; \
	long long TYPE ## shmem_longlong_fadd(long long *target, long long value, int pe) \
		{ FUNC_BODY_ANY(long long, TYPE, shmem_longlong_fadd, target, value, pe) }; \
\
	int TYPE ## shmem_int_finc(int *target, int pe) \
		{ FUNC_BODY_ANY(int, TYPE, shmem_int_finc, target, pe) }; \
	long TYPE ## shmem_long_finc(long *target, int pe) \
		{ FUNC_BODY_ANY(long, TYPE, shmem_long_finc, target, pe) }; \
	long long TYPE ## shmem_longlong_finc(long long *target, int pe) \
		{ FUNC_BODY_ANY(long long, TYPE, shmem_longlong_finc, target, pe) }; \
\
	void TYPE ## shmem_int_add(int *target, int value, int pe) \
		{ FUNC_BODY_VOID(TYPE, shmem_int_add, target, value, pe) }; \
	void TYPE ## shmem_long_add(long *target, long value, int pe) \
		{ FUNC_BODY_VOID(TYPE, shmem_long_add, target, value, pe) }; \
	void TYPE ## shmem_longlong_add(long long *target, long long value, int pe) \
		{ FUNC_BODY_VOID(TYPE, shmem_longlong_add, target, value, pe) }; \
\
	void TYPE ## shmem_int_inc(int *target, int pe) \
		{ FUNC_BODY_VOID(TYPE, shmem_int_inc, target, pe) }; \
	void TYPE ## shmem_long_inc(long *target, int pe) \
		{ FUNC_BODY_VOID(TYPE, shmem_long_inc, target, pe) }; \
	void TYPE ## shmem_longlong_inc(long long *target, int pe) \
		{ FUNC_BODY_VOID(TYPE, shmem_longlong_inc, target, pe) }; \
\
	void TYPE ## shmem_set_lock(volatile long *lock) \
		{ FUNC_BODY_VOID(TYPE, shmem_set_lock, lock) }; \
	void TYPE ## shmem_clear_lock(volatile long *lock) \
		{ FUNC_BODY_VOID(TYPE, shmem_clear_lock, lock) }; \
	int TYPE ## shmem_test_lock(volatile long *lock) \
		{ FUNC_BODY_INT(TYPE, shmem_test_lock, lock) }; \
\
	void TYPE ## shmem_short_wait(volatile short *addr, short value) \
		{ FUNC_BODY_VOID(TYPE, shmem_short_wait, addr, value) }; \
	void TYPE ## shmem_int_wait(volatile int *addr, int value) \
		{ FUNC_BODY_VOID(TYPE, shmem_int_wait, addr, value) }; \
	void TYPE ## shmem_long_wait(volatile long *addr, long value) \
		{ FUNC_BODY_VOID(TYPE, shmem_long_wait, addr, value) }; \
	void TYPE ## shmem_longlong_wait(volatile long long *addr, long long value) \
		{ FUNC_BODY_VOID(TYPE, shmem_longlong_wait, addr, value) }; \
	void TYPE ## shmem_wait(volatile long *addr, long value) \
		{ FUNC_BODY_VOID(TYPE, shmem_wait, addr, value) }; \
	void TYPE ## shmem_short_wait_until(volatile short *addr, int cmp, short value) \
		{ FUNC_BODY_VOID(TYPE, shmem_short_wait_until, addr, cmp, value) }; \
	void TYPE ## shmem_int_wait_until(volatile int *addr, int cmp, int value) \
		{ FUNC_BODY_VOID(TYPE, shmem_int_wait_until, addr, cmp, value) }; \
	void TYPE ## shmem_long_wait_until(volatile long *addr, int cmp, long value) \
		{ FUNC_BODY_VOID(TYPE, shmem_long_wait_until, addr, cmp, value) }; \
	void TYPE ## shmem_longlong_wait_until(volatile long long *addr, int cmp, long long value) \
		{ FUNC_BODY_VOID(TYPE, shmem_longlong_wait_until, addr, cmp, value) }; \
	void TYPE ## shmem_wait_until(volatile long *addr, int cmp, long value) \
		{ FUNC_BODY_VOID(TYPE, shmem_wait_until, addr, cmp, value) }; \
\
	void TYPE ## shmem_barrier(int PE_start, int logPE_stride, int PE_size, long *pSync) \
		{ FUNC_BODY_VOID(TYPE, shmem_barrier, PE_start, logPE_stride, PE_size, pSync) }; \
	void TYPE ## shmem_barrier_all(void) \
		{ FUNC_BODY_VOID(TYPE, shmem_barrier_all) }; \
	void TYPE ## shmem_fence(void) \
		{ FUNC_BODY_VOID(TYPE, shmem_fence) }; \
	void TYPE ## shmem_quiet(void) \
		{ FUNC_BODY_VOID(TYPE, shmem_quiet) }; \
\
	void TYPE ## shmem_broadcast32(void *target, const void *source, size_t nlong, int PE_root, int PE_start, int logPE_stride, int PE_size, long *pSync) \
		{ FUNC_BODY_VOID(TYPE, shmem_broadcast32, target, source, nlong, PE_root, PE_start, logPE_stride, PE_size, pSync) }; \
	void TYPE ## shmem_broadcast64(void *target, const void *source, size_t nlong, int PE_root, int PE_start, int logPE_stride, int PE_size, long *pSync) \
		{ FUNC_BODY_VOID(TYPE, shmem_broadcast64, target, source, nlong, PE_root, PE_start, logPE_stride, PE_size, pSync) }; \
	void TYPE ## shmem_broadcast(void *target, const void *source, size_t nlong, int PE_root, int PE_start, int logPE_stride, int PE_size, long *pSync) \
		{ FUNC_BODY_VOID(TYPE, shmem_broadcast, target, source, nlong, PE_root, PE_start, logPE_stride, PE_size, pSync) }; \
	void TYPE ## shmem_collect32(void *target, const void *source, size_t nlong, int PE_start, int logPE_stride, int PE_size, long *pSync) \
		{ FUNC_BODY_VOID(TYPE, shmem_collect32, target, source, nlong, PE_start, logPE_stride, PE_size, pSync) }; \
	void TYPE ## shmem_collect64(void *target, const void *source, size_t nlong, int PE_start, int logPE_stride, int PE_size, long *pSync) \
		{ FUNC_BODY_VOID(TYPE, shmem_collect64, target, source, nlong, PE_start, logPE_stride, PE_size, pSync) }; \
	void TYPE ## shmem_fcollect32(void *target, const void *source, size_t nlong, int PE_start, int logPE_stride, int PE_size, long *pSync) \
		{ FUNC_BODY_VOID(TYPE, shmem_fcollect32, target, source, nlong, PE_start, logPE_stride, PE_size, pSync) }; \
	void TYPE ## shmem_fcollect64(void *target, const void *source, size_t nlong, int PE_start, int logPE_stride, int PE_size, long *pSync) \
		{ FUNC_BODY_VOID(TYPE, shmem_fcollect64, target, source, nlong, PE_start, logPE_stride, PE_size, pSync) }; \
\
	void TYPE ## shmem_short_and_to_all(short *target, const short *source, int nreduce, int PE_start, int logPE_stride, int PE_size, short *pWrk, long *pSync) \
		{ FUNC_BODY_VOID(TYPE, shmem_short_and_to_all, target, source, nreduce, PE_start, logPE_stride, PE_size, pWrk, pSync) }; \
	void TYPE ## shmem_int_and_to_all(int *target, const int *source, int nreduce, int PE_start, int logPE_stride, int PE_size, int *pWrk, long *pSync) \
		{ FUNC_BODY_VOID(TYPE, shmem_int_and_to_all, target, source, nreduce, PE_start, logPE_stride, PE_size, pWrk, pSync) }; \
	void TYPE ## shmem_long_and_to_all(long *target, const long *source, int nreduce, int PE_start, int logPE_stride, int PE_size, long *pWrk, long *pSync) \
		{ FUNC_BODY_VOID(TYPE, shmem_long_and_to_all, target, source, nreduce, PE_start, logPE_stride, PE_size, pWrk, pSync) }; \
	void TYPE ## shmem_longlong_and_to_all(long long *target, const long long *source, int nreduce, int PE_start, int logPE_stride, int PE_size, long long *pWrk, long *pSync) \
		{ FUNC_BODY_VOID(TYPE, shmem_longlong_and_to_all, target, source, nreduce, PE_start, logPE_stride, PE_size, pWrk, pSync) }; \
\
	void TYPE ## shmem_short_or_to_all(short *target, const short *source, int nreduce, int PE_start, int logPE_stride, int PE_size, short *pWrk, long *pSync) \
		{ FUNC_BODY_VOID(TYPE, shmem_short_or_to_all, target, source, nreduce, PE_start, logPE_stride, PE_size, pWrk, pSync) }; \
	void TYPE ## shmem_int_or_to_all(int *target, const int *source, int nreduce, int PE_start, int logPE_stride, int PE_size, int *pWrk, long *pSync) \
		{ FUNC_BODY_VOID(TYPE, shmem_int_or_to_all, target, source, nreduce, PE_start, logPE_stride, PE_size, pWrk, pSync) }; \
	void TYPE ## shmem_long_or_to_all(long *target, const long *source, int nreduce, int PE_start, int logPE_stride, int PE_size, long *pWrk, long *pSync) \
		{ FUNC_BODY_VOID(TYPE, shmem_long_or_to_all, target, source, nreduce, PE_start, logPE_stride, PE_size, pWrk, pSync) }; \
	void TYPE ## shmem_longlong_or_to_all(long long *target, const long long *source, int nreduce, int PE_start, int logPE_stride, int PE_size, long long *pWrk, long *pSync) \
		{ FUNC_BODY_VOID(TYPE, shmem_longlong_or_to_all, target, source, nreduce, PE_start, logPE_stride, PE_size, pWrk, pSync) }; \
\
	void TYPE ## shmem_short_xor_to_all(short *target, const short *source, int nreduce, int PE_start, int logPE_stride, int PE_size, short *pWrk, long *pSync) \
		{ FUNC_BODY_VOID(TYPE, shmem_short_xor_to_all, target, source, nreduce, PE_start, logPE_stride, PE_size, pWrk, pSync) }; \
	void TYPE ## shmem_int_xor_to_all(int *target, const int *source, int nreduce, int PE_start, int logPE_stride, int PE_size, int *pWrk, long *pSync) \
		{ FUNC_BODY_VOID(TYPE, shmem_int_xor_to_all, target, source, nreduce, PE_start, logPE_stride, PE_size, pWrk, pSync) }; \
	void TYPE ## shmem_long_xor_to_all(long *target, const long *source, int nreduce, int PE_start, int logPE_stride, int PE_size, long *pWrk, long *pSync) \
		{ FUNC_BODY_VOID(TYPE, shmem_long_xor_to_all, target, source, nreduce, PE_start, logPE_stride, PE_size, pWrk, pSync) }; \
	void TYPE ## shmem_longlong_xor_to_all(long long *target, const long long *source, int nreduce, int PE_start, int logPE_stride, int PE_size, long long *pWrk, long *pSync) \
		{ FUNC_BODY_VOID(TYPE, shmem_longlong_xor_to_all, target, source, nreduce, PE_start, logPE_stride, PE_size, pWrk, pSync) }; \
\
	void TYPE ## shmem_short_max_to_all(short *target, const short *source, int nreduce, int PE_start, int logPE_stride, int PE_size, short *pWrk, long *pSync) \
		{ FUNC_BODY_VOID(TYPE, shmem_short_max_to_all, target, source, nreduce, PE_start, logPE_stride, PE_size, pWrk, pSync) }; \
	void TYPE ## shmem_int_max_to_all(int *target, const int *source, int nreduce, int PE_start, int logPE_stride, int PE_size, int *pWrk, long *pSync) \
		{ FUNC_BODY_VOID(TYPE, shmem_int_max_to_all, target, source, nreduce, PE_start, logPE_stride, PE_size, pWrk, pSync) }; \
	void TYPE ## shmem_long_max_to_all(long *target, const long *source, int nreduce, int PE_start, int logPE_stride, int PE_size, long *pWrk, long *pSync) \
		{ FUNC_BODY_VOID(TYPE, shmem_long_max_to_all, target, source, nreduce, PE_start, logPE_stride, PE_size, pWrk, pSync) }; \
	void TYPE ## shmem_longlong_max_to_all(long long *target, const long long *source, int nreduce, int PE_start, int logPE_stride, int PE_size, long long *pWrk, long *pSync) \
		{ FUNC_BODY_VOID(TYPE, shmem_longlong_max_to_all, target, source, nreduce, PE_start, logPE_stride, PE_size, pWrk, pSync) }; \
	void TYPE ## shmem_float_max_to_all(float *target, const float *source, int nreduce, int PE_start, int logPE_stride, int PE_size, float *pWrk, long *pSync) \
		{ FUNC_BODY_VOID(TYPE, shmem_float_max_to_all, target, source, nreduce, PE_start, logPE_stride, PE_size, pWrk, pSync) }; \
	void TYPE ## shmem_double_max_to_all(double *target, const double *source, int nreduce, int PE_start, int logPE_stride, int PE_size, double *pWrk, long *pSync) \
		{ FUNC_BODY_VOID(TYPE, shmem_double_max_to_all, target, source, nreduce, PE_start, logPE_stride, PE_size, pWrk, pSync) }; \
	void TYPE ## shmem_longdouble_max_to_all(long double *target, const long double *source, int nreduce, int PE_start, int logPE_stride, int PE_size, long double *pWrk, long *pSync) \
		{ FUNC_BODY_VOID(TYPE, shmem_longdouble_max_to_all, target, source, nreduce, PE_start, logPE_stride, PE_size, pWrk, pSync) }; \
\
	void TYPE ## shmem_short_min_to_all(short *target, const short *source, int nreduce, int PE_start, int logPE_stride, int PE_size, short *pWrk, long *pSync) \
		{ FUNC_BODY_VOID(TYPE, shmem_short_min_to_all, target, source, nreduce, PE_start, logPE_stride, PE_size, pWrk, pSync) }; \
	void TYPE ## shmem_int_min_to_all(int *target, const int *source, int nreduce, int PE_start, int logPE_stride, int PE_size, int *pWrk, long *pSync) \
		{ FUNC_BODY_VOID(TYPE, shmem_int_min_to_all, target, source, nreduce, PE_start, logPE_stride, PE_size, pWrk, pSync) }; \
	void TYPE ## shmem_long_min_to_all(long *target, const long *source, int nreduce, int PE_start, int logPE_stride, int PE_size, long *pWrk, long *pSync) \
		{ FUNC_BODY_VOID(TYPE, shmem_long_min_to_all, target, source, nreduce, PE_start, logPE_stride, PE_size, pWrk, pSync) }; \
	void TYPE ## shmem_longlong_min_to_all(long long *target, const long long *source, int nreduce, int PE_start, int logPE_stride, int PE_size, long long *pWrk, long *pSync) \
		{ FUNC_BODY_VOID(TYPE, shmem_longlong_min_to_all, target, source, nreduce, PE_start, logPE_stride, PE_size, pWrk, pSync) }; \
	void TYPE ## shmem_float_min_to_all(float *target, const float *source, int nreduce, int PE_start, int logPE_stride, int PE_size, float *pWrk, long *pSync) \
		{ FUNC_BODY_VOID(TYPE, shmem_float_min_to_all, target, source, nreduce, PE_start, logPE_stride, PE_size, pWrk, pSync) }; \
	void TYPE ## shmem_double_min_to_all(double *target, const double *source, int nreduce, int PE_start, int logPE_stride, int PE_size, double *pWrk, long *pSync) \
		{ FUNC_BODY_VOID(TYPE, shmem_double_min_to_all, target, source, nreduce, PE_start, logPE_stride, PE_size, pWrk, pSync) }; \
	void TYPE ## shmem_longdouble_min_to_all(long double *target, const long double *source, int nreduce, int PE_start, int logPE_stride, int PE_size, long double *pWrk, long *pSync) \
		{ FUNC_BODY_VOID(TYPE, shmem_longdouble_min_to_all, target, source, nreduce, PE_start, logPE_stride, PE_size, pWrk, pSync) }; \
\
	void TYPE ## shmem_short_sum_to_all(short *target, const short *source, int nreduce, int PE_start, int logPE_stride, int PE_size, short *pWrk, long *pSync) \
		{ FUNC_BODY_VOID(TYPE, shmem_short_sum_to_all, target, source, nreduce, PE_start, logPE_stride, PE_size, pWrk, pSync) }; \
	void TYPE ## shmem_int_sum_to_all(int *target, const int *source, int nreduce, int PE_start, int logPE_stride, int PE_size, int *pWrk, long *pSync) \
		{ FUNC_BODY_VOID(TYPE, shmem_int_sum_to_all, target, source, nreduce, PE_start, logPE_stride, PE_size, pWrk, pSync) }; \
	void TYPE ## shmem_long_sum_to_all(long *target, const long *source, int nreduce, int PE_start, int logPE_stride, int PE_size, long *pWrk, long *pSync) \
		{ FUNC_BODY_VOID(TYPE, shmem_long_sum_to_all, target, source, nreduce, PE_start, logPE_stride, PE_size, pWrk, pSync) }; \
	void TYPE ## shmem_longlong_sum_to_all(long long *target, const long long *source, int nreduce, int PE_start, int logPE_stride, int PE_size, long long *pWrk, long *pSync) \
		{ FUNC_BODY_VOID(TYPE, shmem_longlong_sum_to_all, target, source, nreduce, PE_start, logPE_stride, PE_size, pWrk, pSync) }; \
	void TYPE ## shmem_float_sum_to_all(float *target, const float *source, int nreduce, int PE_start, int logPE_stride, int PE_size, float *pWrk, long *pSync) \
		{ FUNC_BODY_VOID(TYPE, shmem_float_sum_to_all, target, source, nreduce, PE_start, logPE_stride, PE_size, pWrk, pSync) }; \
	void TYPE ## shmem_double_sum_to_all(double *target, const double *source, int nreduce, int PE_start, int logPE_stride, int PE_size, double *pWrk, long *pSync) \
		{ FUNC_BODY_VOID(TYPE, shmem_double_sum_to_all, target, source, nreduce, PE_start, logPE_stride, PE_size, pWrk, pSync) }; \
	void TYPE ## shmem_longdouble_sum_to_all(long double *target, const long double *source, int nreduce, int PE_start, int logPE_stride, int PE_size, long double *pWrk, long *pSync) \
		{ FUNC_BODY_VOID(TYPE, shmem_longdouble_sum_to_all, target, source, nreduce, PE_start, logPE_stride, PE_size, pWrk, pSync) }; \
	void TYPE ## shmem_complexf_sum_to_all(OSHMEM_COMPLEX_TYPE(float) *target, const OSHMEM_COMPLEX_TYPE(float) *source, int nreduce, int PE_start, int logPE_stride, int PE_size, OSHMEM_COMPLEX_TYPE(float) *pWrk, long *pSync) \
		{ FUNC_BODY_VOID(TYPE, shmem_complexf_sum_to_all, target, source, nreduce, PE_start, logPE_stride, PE_size, pWrk, pSync) }; \
	void TYPE ## shmem_complexd_sum_to_all(OSHMEM_COMPLEX_TYPE(double) *target, const OSHMEM_COMPLEX_TYPE(double) *source, int nreduce, int PE_start, int logPE_stride, int PE_size, OSHMEM_COMPLEX_TYPE(double) *pWrk, long *pSync) \
		{ FUNC_BODY_VOID(TYPE, shmem_complexd_sum_to_all, target, source, nreduce, PE_start, logPE_stride, PE_size, pWrk, pSync) }; \
\
	void TYPE ## shmem_short_prod_to_all(short *target, const short *source, int nreduce, int PE_start, int logPE_stride, int PE_size, short *pWrk, long *pSync) \
		{ FUNC_BODY_VOID(TYPE, shmem_short_prod_to_all, target, source, nreduce, PE_start, logPE_stride, PE_size, pWrk, pSync) }; \
	void TYPE ## shmem_int_prod_to_all(int *target, const int *source, int nreduce, int PE_start, int logPE_stride, int PE_size, int *pWrk, long *pSync) \
		{ FUNC_BODY_VOID(TYPE, shmem_int_prod_to_all, target, source, nreduce, PE_start, logPE_stride, PE_size, pWrk, pSync) }; \
	void TYPE ## shmem_long_prod_to_all(long *target, const long *source, int nreduce, int PE_start, int logPE_stride, int PE_size, long *pWrk, long *pSync) \
		{ FUNC_BODY_VOID(TYPE, shmem_long_prod_to_all, target, source, nreduce, PE_start, logPE_stride, PE_size, pWrk, pSync) }; \
	void TYPE ## shmem_longlong_prod_to_all(long long *target, const long long *source, int nreduce, int PE_start, int logPE_stride, int PE_size, long long *pWrk, long *pSync) \
		{ FUNC_BODY_VOID(TYPE, shmem_longlong_prod_to_all, target, source, nreduce, PE_start, logPE_stride, PE_size, pWrk, pSync) }; \
	void TYPE ## shmem_float_prod_to_all(float *target, const float *source, int nreduce, int PE_start, int logPE_stride, int PE_size, float *pWrk, long *pSync) \
		{ FUNC_BODY_VOID(TYPE, shmem_float_prod_to_all, target, source, nreduce, PE_start, logPE_stride, PE_size, pWrk, pSync) }; \
	void TYPE ## shmem_double_prod_to_all(double *target, const double *source, int nreduce, int PE_start, int logPE_stride, int PE_size, double *pWrk, long *pSync) \
		{ FUNC_BODY_VOID(TYPE, shmem_double_prod_to_all, target, source, nreduce, PE_start, logPE_stride, PE_size, pWrk, pSync) }; \
	void TYPE ## shmem_longdouble_prod_to_all(long double *target, const long double *source, int nreduce, int PE_start, int logPE_stride, int PE_size, long double *pWrk, long *pSync) \
		{ FUNC_BODY_VOID(TYPE, shmem_longdouble_prod_to_all, target, source, nreduce, PE_start, logPE_stride, PE_size, pWrk, pSync) }; \
	void TYPE ## shmem_complexf_prod_to_all(OSHMEM_COMPLEX_TYPE(float) *target, const OSHMEM_COMPLEX_TYPE(float) *source, int nreduce, int PE_start, int logPE_stride, int PE_size, OSHMEM_COMPLEX_TYPE(float) *pWrk, long *pSync) \
		{ FUNC_BODY_VOID(TYPE, shmem_complexf_prod_to_all, target, source, nreduce, PE_start, logPE_stride, PE_size, pWrk, pSync) }; \
	void TYPE ## shmem_complexd_prod_to_all(OSHMEM_COMPLEX_TYPE(double) *target, const OSHMEM_COMPLEX_TYPE(double) *source, int nreduce, int PE_start, int logPE_stride, int PE_size, OSHMEM_COMPLEX_TYPE(double) *pWrk, long *pSync) \
		{ FUNC_BODY_VOID(TYPE, shmem_complexd_prod_to_all, target, source, nreduce, PE_start, logPE_stride, PE_size, pWrk, pSync) }; \
\
	void TYPE ## shmem_udcflush(void) \
		{ FUNC_BODY_VOID(TYPE, shmem_udcflush) }; \
	void TYPE ## shmem_udcflush_line(void* target) \
		{ FUNC_BODY_VOID(TYPE, shmem_udcflush_line, target) }; \
	void TYPE ## shmem_set_cache_inv(void) \
		{ FUNC_BODY_VOID(TYPE, shmem_set_cache_inv) }; \
	void TYPE ## shmem_set_cache_line_inv(void* target) \
		{ FUNC_BODY_VOID(TYPE, shmem_set_cache_line_inv, target) }; \
	void TYPE ## shmem_clear_cache_inv(void) \
		{ FUNC_BODY_VOID(TYPE, shmem_clear_cache_inv) }; \
	void TYPE ## shmem_clear_cache_line_inv(void* target) \
		{ FUNC_BODY_VOID(TYPE, shmem_clear_cache_line_inv, target) };

#define DECLARE_OPTION_FUNCTIONS_PROTOTYPED_EX(TYPE) \
\
	void TYPE ## shmem_char_put_nbi(char *target, const char *source, size_t len, int pe) \
		{ FUNC_BODY_VOID(TYPE, shmem_char_put_nbi, target, source, len, pe) }; \
	void TYPE ## shmem_short_put_nbi(short *target, const short *source, size_t len, int pe) \
		{ FUNC_BODY_VOID(TYPE, shmem_short_put_nbi, target, source, len, pe) }; \
	void TYPE ## shmem_int_put_nbi(int *target, const int *source, size_t len, int pe) \
		{ FUNC_BODY_VOID(TYPE, shmem_int_put_nbi, target, source, len, pe) }; \
	void TYPE ## shmem_long_put_nbi(long *target, const long *source, size_t len, int pe) \
		{ FUNC_BODY_VOID(TYPE, shmem_long_put_nbi, target, source, len, pe) }; \
	void TYPE ## shmem_float_put_nbi(float *target, const float *source, size_t len, int pe) \
		{ FUNC_BODY_VOID(TYPE, shmem_float_put_nbi, target, source, len, pe) }; \
	void TYPE ## shmem_double_put_nbi(double *target, const double *source, size_t len, int pe) \
		{ FUNC_BODY_VOID(TYPE, shmem_double_put_nbi, target, source, len, pe) }; \
	void TYPE ## shmem_longlong_put_nbi(long long *target, const long long *source, size_t len, int pe) \
		{ FUNC_BODY_VOID(TYPE, shmem_longlong_put_nbi, target, source, len, pe) }; \
	void TYPE ## shmem_longdouble_put_nbi(long double *target, const long double *source, size_t len, int pe) \
		{ FUNC_BODY_VOID(TYPE, shmem_longdouble_put_nbi, target, source, len, pe) }; \
	void TYPE ## shmem_put32_nbi(void *target, const void *source, size_t len, int pe) \
		{ FUNC_BODY_VOID(TYPE, shmem_put32_nbi, target, source, len, pe) }; \
	void TYPE ## shmem_put64_nbi(void *target, const void *source, size_t len, int pe) \
		{ FUNC_BODY_VOID(TYPE, shmem_put64_nbi, target, source, len, pe) }; \
	void TYPE ## shmem_put128_nbi(void *target, const void *source, size_t len, int pe) \
		{ FUNC_BODY_VOID(TYPE, shmem_put128_nbi, target, source, len, pe) }; \
	void TYPE ## shmem_putmem_nbi(void *target, const void *source, size_t len, int pe) \
		{ FUNC_BODY_VOID(TYPE, shmem_putmem_nbi, target, source, len, pe) }; \
\
	void TYPE ## shmem_char_get_nbi(char *target, const char *source, size_t len, int pe) \
		{ FUNC_BODY_VOID(TYPE, shmem_char_get_nbi, target, source, len, pe) }; \
	void TYPE ## shmem_short_get_nbi(short *target, const short *source, size_t len, int pe) \
		{ FUNC_BODY_VOID(TYPE, shmem_short_get_nbi, target, source, len, pe) }; \
	void TYPE ## shmem_int_get_nbi(int *target, const int *source, size_t len, int pe) \
		{ FUNC_BODY_VOID(TYPE, shmem_int_get_nbi, target, source, len, pe) }; \
	void TYPE ## shmem_long_get_nbi(long *target, const long *source, size_t len, int pe) \
		{ FUNC_BODY_VOID(TYPE, shmem_long_get_nbi, target, source, len, pe) }; \
	void TYPE ## shmem_float_get_nbi(float *target, const float *source, size_t len, int pe) \
		{ FUNC_BODY_VOID(TYPE, shmem_float_get_nbi, target, source, len, pe) }; \
	void TYPE ## shmem_double_get_nbi(double *target, const double *source, size_t len, int pe) \
		{ FUNC_BODY_VOID(TYPE, shmem_double_get_nbi, target, source, len, pe) }; \
	void TYPE ## shmem_longlong_get_nbi(long long *target, const long long *source, size_t len, int pe) \
		{ FUNC_BODY_VOID(TYPE, shmem_longlong_get_nbi, target, source, len, pe) }; \
	void TYPE ## shmem_longdouble_get_nbi(long double *target, const long double *source, size_t len, int pe) \
		{ FUNC_BODY_VOID(TYPE, shmem_longdouble_get_nbi, target, source, len, pe) }; \
	void TYPE ## shmem_get32_nbi(void *target, const void *source, size_t len, int pe) \
		{ FUNC_BODY_VOID(TYPE, shmem_get32_nbi, target, source, len, pe) }; \
	void TYPE ## shmem_get64_nbi(void *target, const void *source, size_t len, int pe) \
		{ FUNC_BODY_VOID(TYPE, shmem_get64_nbi, target, source, len, pe) }; \
	void TYPE ## shmem_get128_nbi(void *target, const void *source, size_t len, int pe) \
		{ FUNC_BODY_VOID(TYPE, shmem_get128_nbi, target, source, len, pe) }; \
	void TYPE ## shmem_getmem_nbi(void *target, const void *source, size_t len, int pe) \
		{ FUNC_BODY_VOID(TYPE, shmem_getmem_nbi, target, source, len, pe) }; \
\
	void TYPE ## shmem_alltoall32(void *target, const void *source, size_t nlong, int PE_start, int logPE_stride, int PE_size, long *pSync) \
		{ FUNC_BODY_VOID(TYPE, shmem_alltoall32, target, source, nlong, PE_start, logPE_stride, PE_size, pSync) }; \
	void TYPE ## shmem_alltoall64(void *target, const void *source, size_t nlong, int PE_start, int logPE_stride, int PE_size, long *pSync) \
		{ FUNC_BODY_VOID(TYPE, shmem_alltoall64, target, source, nlong, PE_start, logPE_stride, PE_size, pSync) }; \
	void TYPE ## shmem_alltoalls32(void *target, const void *source, ptrdiff_t dst, ptrdiff_t sst, size_t nlong, int PE_start, int logPE_stride, int PE_size, long *pSync) \
		{ FUNC_BODY_VOID(TYPE, shmem_alltoall32, target, source, dst, sst, nlong, PE_start, logPE_stride, PE_size, pSync) }; \
	void TYPE ## shmem_alltoalls64(void *target, const void *source, ptrdiff_t dst, ptrdiff_t sst, size_t nlong, int PE_start, int logPE_stride, int PE_size, long *pSync) \
		{ FUNC_BODY_VOID(TYPE, shmem_alltoall64, target, source, dst, sst, nlong, PE_start, logPE_stride, PE_size, pSync) }; \


/****************************************************************************
 * Module declaration place
 ***************************************************************************/

#define DECLARE_OPTION(type) \
DECLARE_OPTION_FUNCTIONS_PROTOTYPED(type) \
DECLARE_OPTION_STRUCT(type)

#if defined(HAVE_VISIBILITY)
#pragma GCC visibility push(default)
#endif
DECLARE_OPTION_FUNCTIONS_PROTOTYPED( )
#if defined(HAVE_VISIBILITY)
#pragma GCC visibility pop
#endif

#define PREFIX_NONE(x) NONE##x,
DECLARE_OPTION(NONE)

#define PREFIX_PROF(x) PROF##x,
DECLARE_OPTION(PROF)

#define PREFIX_VERBOSE(x) VERBOSE##x,
DECLARE_OPTION(VERBOSE)

#define PREFIX_TRACE(x) TRACE##x,
DECLARE_OPTION(TRACE)

#define PREFIX_ERR(x) ERR##x,
DECLARE_OPTION(ERR)

/****************************************************************************
 * Module configuration place
 ***************************************************************************/

static const IBPROF_MODULE_CALL shmem_tbl_call[] =
{
	OP_ON_MEMBERS_LIST(TBL_CALL_ENRTY)
	{UNDEFINED_VALUE, NULL, NULL},
};

static IBPROF_ERROR __shmem_init(IBPROF_MODULE_OBJECT *mod_obj)
{
	IBPROF_ERROR status = IBPROF_ERR_NONE;
	struct shmem_module_api_t* src_api = NULL;

	if ((status = sys_dlcheck("liboshmem.so")) != IBPROF_ERR_NONE)
		return status;

	/*
	 * Find the original version of functions we override.
	 */
	check_dlsym(shmem_init);
	check_dlsym(shmem_finalize);

	check_dlsym(shmem_n_pes);
	check_dlsym(shmem_my_pe);

	check_dlsym(shmem_pe_accessible);
	check_dlsym(shmem_addr_accessible);

	check_dlsym(shmem_malloc);
	check_dlsym(shmem_align);
	check_dlsym(shmem_realloc);
	check_dlsym(shmem_free);

	check_dlsym(shmem_ptr);

	check_dlsym(shmem_char_p);
	check_dlsym(shmem_short_p);
	check_dlsym(shmem_int_p);
	check_dlsym(shmem_long_p);
	check_dlsym(shmem_float_p);
	check_dlsym(shmem_double_p);
	check_dlsym(shmem_longlong_p);
	check_dlsym(shmem_longdouble_p);

	check_dlsym(shmem_short_put);
	check_dlsym(shmem_int_put);
	check_dlsym(shmem_long_put);
	check_dlsym(shmem_float_put);
	check_dlsym(shmem_double_put);
	check_dlsym(shmem_longlong_put);
	check_dlsym(shmem_longdouble_put);
	check_dlsym(shmem_put32);
	check_dlsym(shmem_put64);
	check_dlsym(shmem_put128);
	check_dlsym(shmem_putmem);

	check_dlsym(shmem_int_iput);
	check_dlsym(shmem_short_iput);
	check_dlsym(shmem_float_iput);
	check_dlsym(shmem_double_iput);
	check_dlsym(shmem_longlong_iput);
	check_dlsym(shmem_longdouble_iput);
	check_dlsym(shmem_long_iput);
	check_dlsym(shmem_iput32);
	check_dlsym(shmem_iput64);
	check_dlsym(shmem_iput128);

#if 0 /* v1.3 nbi/alltoall*/
	check_dlsym(shmem_char_put_nbi);
	check_dlsym(shmem_short_put_nbi);
	check_dlsym(shmem_int_put_nbi);
	check_dlsym(shmem_long_put_nbi);
	check_dlsym(shmem_float_put_nbi);
	check_dlsym(shmem_double_put_nbi);
	check_dlsym(shmem_longlong_put_nbi);
	check_dlsym(shmem_longdouble_put_nbi);
	check_dlsym(shmem_put8_nbi);
	check_dlsym(shmem_put16_nbi);
	check_dlsym(shmem_put32_nbi);
	check_dlsym(shmem_put64_nbi);
	check_dlsym(shmem_put128_nbi);
	check_dlsym(shmem_putmem_nbi);
#endif
	check_dlsym(shmem_char_g);
	check_dlsym(shmem_short_g);
	check_dlsym(shmem_int_g);
	check_dlsym(shmem_long_g);
	check_dlsym(shmem_float_g);
	check_dlsym(shmem_double_g);
	check_dlsym(shmem_longlong_g);
	check_dlsym(shmem_longdouble_g);

	check_dlsym(shmem_short_get);
	check_dlsym(shmem_int_get);
	check_dlsym(shmem_long_get);
	check_dlsym(shmem_float_get);
	check_dlsym(shmem_double_get);
	check_dlsym(shmem_longlong_get);
	check_dlsym(shmem_longdouble_get);
	check_dlsym(shmem_get32);
	check_dlsym(shmem_get64);
	check_dlsym(shmem_get128);
	check_dlsym(shmem_getmem);

	check_dlsym(shmem_int_iget);
	check_dlsym(shmem_short_iget);
	check_dlsym(shmem_float_iget);
	check_dlsym(shmem_double_iget);
	check_dlsym(shmem_longlong_iget);
	check_dlsym(shmem_longdouble_iget);
	check_dlsym(shmem_long_iget);
	check_dlsym(shmem_iget32);
	check_dlsym(shmem_iget64);
	check_dlsym(shmem_iget128);
#if 0 /* v1.3 nbi/alltoall*/
	check_dlsym(shmem_char_get_nbi);
	check_dlsym(shmem_short_get_nbi);
	check_dlsym(shmem_int_get_nbi);
	check_dlsym(shmem_long_get_nbi);
	check_dlsym(shmem_float_get_nbi);
	check_dlsym(shmem_double_get_nbi);
	check_dlsym(shmem_longlong_get_nbi);
	check_dlsym(shmem_longdouble_get_nbi);
	check_dlsym(shmem_get8_nbi);
	check_dlsym(shmem_get16_nbi);
	check_dlsym(shmem_get32_nbi);
	check_dlsym(shmem_get64_nbi);
	check_dlsym(shmem_get128_nbi);
	check_dlsym(shmem_getmem_nbi);
#endif

	check_dlsym(shmem_swap);
	check_dlsym(shmem_double_swap);
	check_dlsym(shmem_float_swap);
	check_dlsym(shmem_int_swap);
	check_dlsym(shmem_long_swap);
	check_dlsym(shmem_longlong_swap);

	check_dlsym(shmem_int_cswap);
	check_dlsym(shmem_long_cswap);
	check_dlsym(shmem_longlong_cswap);

	check_dlsym(shmem_int_fadd);
	check_dlsym(shmem_long_fadd);
	check_dlsym(shmem_longlong_fadd);

	check_dlsym(shmem_int_finc);
	check_dlsym(shmem_long_finc);
	check_dlsym(shmem_longlong_finc);

	check_dlsym(shmem_int_add);
	check_dlsym(shmem_long_add);
	check_dlsym(shmem_longlong_add);

	check_dlsym(shmem_int_inc);
	check_dlsym(shmem_long_inc);
	check_dlsym(shmem_longlong_inc);

	check_dlsym(shmem_set_lock);
	check_dlsym(shmem_clear_lock);
	check_dlsym(shmem_test_lock);

	check_dlsym(shmem_short_wait);
	check_dlsym(shmem_int_wait);
	check_dlsym(shmem_long_wait);
	check_dlsym(shmem_longlong_wait);
	check_dlsym(shmem_wait);
	check_dlsym(shmem_short_wait_until);
	check_dlsym(shmem_int_wait_until);
	check_dlsym(shmem_long_wait_until);
	check_dlsym(shmem_longlong_wait_until);
	check_dlsym(shmem_wait_until);

	check_dlsym(shmem_barrier);
	check_dlsym(shmem_barrier_all);
	check_dlsym(shmem_fence);
	check_dlsym(shmem_quiet);

	check_dlsym(shmem_broadcast32);
	check_dlsym(shmem_broadcast64);
	check_dlsym(shmem_broadcast);
	check_dlsym(shmem_collect32);
	check_dlsym(shmem_collect64);
	check_dlsym(shmem_fcollect32);
	check_dlsym(shmem_fcollect64);
#if 0 /* v1.3 nbi/alltoall*/
	check_dlsym(shmem_alltoall32);
	check_dlsym(shmem_alltoall64);
	check_dlsym(shmem_alltoalls32);
	check_dlsym(shmem_alltoalls64);
#endif
	check_dlsym(shmem_short_and_to_all);
	check_dlsym(shmem_int_and_to_all);
	check_dlsym(shmem_long_and_to_all);
	check_dlsym(shmem_longlong_and_to_all);

	check_dlsym(shmem_short_or_to_all);
	check_dlsym(shmem_int_or_to_all);
	check_dlsym(shmem_long_or_to_all);
	check_dlsym(shmem_longlong_or_to_all);

	check_dlsym(shmem_short_xor_to_all);
	check_dlsym(shmem_int_xor_to_all);
	check_dlsym(shmem_long_xor_to_all);
	check_dlsym(shmem_longlong_xor_to_all);

	check_dlsym(shmem_short_max_to_all);
	check_dlsym(shmem_int_max_to_all);
	check_dlsym(shmem_long_max_to_all);
	check_dlsym(shmem_longlong_max_to_all);
	check_dlsym(shmem_float_max_to_all);
	check_dlsym(shmem_double_max_to_all);
	check_dlsym(shmem_longdouble_max_to_all);

	check_dlsym(shmem_short_min_to_all);
	check_dlsym(shmem_int_min_to_all);
	check_dlsym(shmem_long_min_to_all);
	check_dlsym(shmem_longlong_min_to_all);
	check_dlsym(shmem_float_min_to_all);
	check_dlsym(shmem_double_min_to_all);
	check_dlsym(shmem_longdouble_min_to_all);

	check_dlsym(shmem_short_sum_to_all);
	check_dlsym(shmem_int_sum_to_all);
	check_dlsym(shmem_long_sum_to_all);
	check_dlsym(shmem_longlong_sum_to_all);
	check_dlsym(shmem_float_sum_to_all);
	check_dlsym(shmem_double_sum_to_all);
	check_dlsym(shmem_longdouble_sum_to_all);
	check_dlsym(shmem_complexf_sum_to_all);
	check_dlsym(shmem_complexd_sum_to_all);

	check_dlsym(shmem_short_prod_to_all);
	check_dlsym(shmem_int_prod_to_all);
	check_dlsym(shmem_long_prod_to_all);
	check_dlsym(shmem_longlong_prod_to_all);
	check_dlsym(shmem_float_prod_to_all);
	check_dlsym(shmem_double_prod_to_all);
	check_dlsym(shmem_longdouble_prod_to_all);
	check_dlsym(shmem_complexf_prod_to_all);
	check_dlsym(shmem_complexd_prod_to_all);

	check_dlsym(shmem_udcflush);
	check_dlsym(shmem_udcflush_line);
	check_dlsym(shmem_set_cache_inv);
	check_dlsym(shmem_set_cache_line_inv);
	check_dlsym(shmem_clear_cache_inv);
	check_dlsym(shmem_clear_cache_line_inv);

	switch (ibprof_conf_get_int(IBPROF_MODE_SHMEM)) {
	case IBPROF_MODE_NONE:
		src_api = &shmem_NONE_funcs;
		break;

	case IBPROF_MODE_VERBOSE:
		src_api = &shmem_VERBOSE_funcs;
		break;

	case IBPROF_MODE_PROF:
		src_api = &shmem_PROF_funcs;
		break;

	case IBPROF_MODE_ERR:
		src_api = &shmem_ERR_funcs;
		break;

	case IBPROF_MODE_TRACE:
		src_api = &shmem_TRACE_funcs;
		break;

	default:
		src_api = &shmem_NONE_funcs;
	}

	memcpy(&shmem_module_context.mean, src_api, sizeof(*src_api));

	return status;
}

IBPROF_MODULE_OBJECT shmem_module = {
	IBPROF_MODULE_SHMEM,
	"libshmem",
	"The SHMEM programming model consists of library routines that provide low\\-latency, " \
	"high\\-bandwidth communication for use in highly parallelized scalable programs. The " \
	"routines in the OpenSHMEM application programming interface (API) provide a programming " \
	"model for exchanging data between cooperating parallel processes. The resulting programs " \
	"are similar in style to Message Passing Interface (MPI) programs. The SHMEM API can " \
	"be used either alone or in combination with MPI routines in the same parallel program.",
	shmem_tbl_call,
	__shmem_init,
	NULL,
	(void *)&shmem_module_context
};
#else
IBPROF_MODULE_OBJECT shmem_module = {
	IBPROF_MODULE_INVALID,
	"libshmem",
	NULL,
	NULL,
	NULL,
	NULL,
	NULL
};
#endif /* USE_SHMEM */
