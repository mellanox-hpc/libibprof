/*
 * Copyright (c) 2013-2015 Mellanox Technologies, Inc.
 *                         All rights reserved.
 * $COPYRIGHT$
 *
 * Additional copyrights may follow
 *
 * $HEADER$
 */

/**
 * @file ibprof_cmn.h
 *
 * @brief Place to define common types.
 *
 **/
#ifndef _IBPROF_DEF_H_
#define _IBPROF_DEF_H_

#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <strings.h>
#include <ctype.h>
#include <time.h>
#include <float.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>
#if defined(__LINUX__)
	#include <stdint.h>
	#include <inttypes.h>
	#include <unistd.h>
	#include <dlfcn.h>
	#include <sys/time.h>
	#include <sys/socket.h>
	#include <sys/ioctl.h>
	#include <netinet/in.h>
	#include <arpa/inet.h>
	#include <pthread.h>
#else
	#include <windows.h>
	#include <winsock.h>
	#include <process.h>
#endif

#include "ibprof_conf.h"

#ifdef __cplusplus
extern "C" {
#endif

#define QUOTE(name) #name
#define STR(macro) QUOTE(macro)

#ifndef __MODULE_VERSION
#define __MODULE_VERSION unknown
#endif

#define __MODULE_NAME             "libibprof"
#define __MODULE_COPYRIGHT        "Copyright (C) 2013 Mellanox Technologies Ltd." \
	"\nsee http://www.mellanox.com/"

#ifndef TRUE
#define TRUE (1 == 1)
#endif /* TRUE */

#ifndef FALSE
#define FALSE (1 == 0)
#endif /* FALSE */

#ifndef NULL
#ifdef __cplusplus
#define NULL    0
#else
#define NULL    ((void *)0)
#endif
#endif

#ifndef BOOL
#define BOOL    int
#endif

#define INLINE  __inline

/* This macros should be used in printf to display uintptr_t */
#ifndef PRIXPTR
#define PRIXPTR    "lX"
#endif

#ifndef UNREFERENCED_PARAMETER
#define UNREFERENCED_PARAMETER(P) ((void)P)
#endif

#define UNDEFINED_VALUE (-1)

/**
 * @enum IBPROF_ERROR
 * @brief List of supported error codes.
 */
typedef enum {
	IBPROF_ERR_NONE = 0x0, /**< the function completed */
	IBPROF_ERR_BAD_ARGUMENT, /**< incorrect parameter */
	IBPROF_ERR_INCORRECT, /**< incorrect internal object */
	IBPROF_ERR_UNSUPPORTED, /**< this function is not supported */
	IBPROF_ERR_NOT_EXIST, /**< requested object does not exist */
	IBPROF_ERR_NO_MEMORY, /**< dynamic memory error */
	IBPROF_ERR_FATAL, /**< system fatal error */
	IBPROF_ERR_TIMEOUT, /**< the time limit expires */
	IBPROF_ERR_UNKNOWN /**< general error */
} IBPROF_ERROR;

/**
 * @struct _IBPROF_MODULE_CALL
 * @brief This structure describes call
 */
typedef struct _IBPROF_MODULE_CALL {
	int call;
	const char *name;
	const char *signature;
} IBPROF_MODULE_CALL;

/**
 * @struct _IBPROF_MODULE_OBJECT
 * @brief This structure describes module
 */
typedef struct _IBPROF_MODULE_OBJECT {
	int id;
	const char *name;
	const char *desc;
	const IBPROF_MODULE_CALL *tbl_call;
	IBPROF_ERROR (*init)(struct _IBPROF_MODULE_OBJECT *);
	IBPROF_ERROR (*exit)(struct _IBPROF_MODULE_OBJECT *);
	void *context;
} IBPROF_MODULE_OBJECT;

extern FILE *ibprof_dump_file;

enum {
	IBPROF_MODE_NONE = 0,
	IBPROF_MODE_PROF,
	IBPROF_MODE_ERR,
	IBPROF_MODE_VERBOSE,
	IBPROF_MODE_TRACE
};

enum {
	IBPROF_LOG_FATAL = 1 << 0,
	IBPROF_LOG_ERR = 1 << 1,
	IBPROF_LOG_WARN = 1 << 2,
	IBPROF_LOG_INFO = 1 << 3,
	IBPROF_LOG_TRACE = 1 << 4
};

#define IBPROF_FATAL(fmt, ...)                                       \
	do {                                                             \
		if (ibprof_conf_get_int(IBPROF_TEST_MASK) & IBPROF_LOG_FATAL) \
			fprintf(stderr, "[    FATAL ] " fmt , ##__VA_ARGS__);    \
		exit(EXIT_FAILURE); \
	} while (0)

#define IBPROF_ERROR(fmt, ...)                                       \
	do {                                                             \
		if (ibprof_conf_get_int(IBPROF_TEST_MASK) & IBPROF_LOG_ERR)   \
			fprintf(stderr, "[    ERROR ] " fmt , ##__VA_ARGS__);    \
	} while (0)

#define IBPROF_WARN(fmt, ...)                                        \
	do {                                                             \
		if (ibprof_conf_get_int(IBPROF_TEST_MASK) & IBPROF_LOG_WARN)  \
			fprintf(stderr, "[    WARN  ] " fmt , ##__VA_ARGS__);    \
	} while (0)

#if defined(_DEBUG)
#define IBPROF_INFO(fmt, ...)                                        \
	do {                                                             \
		if (ibprof_conf_get_int(IBPROF_TEST_MASK) & IBPROF_LOG_INFO)  \
		fprintf(stderr, "[    INFO  ] " fmt , ##__VA_ARGS__);        \
	} while (0)

#define IBPROF_TRACE(fmt, ...)                                       \
	do {                                                             \
		if (ibprof_conf_get_int(IBPROF_TEST_MASK) & IBPROF_LOG_TRACE) \
			fprintf(stderr, "[    TRACE ] " fmt , ##__VA_ARGS__);    \
	} while (0)
#else
#define IBPROF_INFO(fmt, ...)       ((void)0)
#define IBPROF_TRACE(fmt, ...)      ((void)0)
#endif /* _DEBUG */


#if defined(HAVE_CHECK)
#define INTERNAL_CHECK() \
	do { \
		if (NULL == f) { \
			IBPROF_FATAL("%s : '%s' Can`t work. Turn on verbose level to see details\n", \
					__FUNCTION__, __MODULE_NAME); \
		} \
	} while (0)
#else
#define INTERNAL_CHECK()
#endif


/**
 * @name ANSI C Library
 * @brief Standart ANSI C library functions
 */
/** @{*/
#define sys_memcpy      memcpy
#define sys_memset      memset
#define sys_strlen      strlen
#define sys_strcpy      strcpy
#define sys_strncpy     strncpy
#define sys_strcmp      strcmp
#define sys_strcasecmp  strcasecmp
#define sys_strstr      strstr
#define sys_strchr      strchr
#define sys_sprintf     sprintf
#define sys_vsnprintf   vsnprintf
#define sys_strtol      strtol

/* Minimum and maximum macros */
#define sys_max(a, b)  (((a) > (b)) ? (a) : (b))
#define sys_min(a, b)  (((a) < (b)) ? (a) : (b))

#if defined(_MSC_VER)
	#ifndef snprintf
	#define snprintf _snprintf
	#endif
	#ifndef strcasecmp
	#define strcasecmp _stricmp
	#endif
#endif
/** @} */

#define sys_snprintf_safe(buf, size, fmt, ...) \
	sys_min(snprintf(buf, size, fmt,  ##__VA_ARGS__), (int)size)

/**
 * @name Synchronization
 * @brief Multi-threading Synchronization operations.
 */
/** @{ */
#if defined(_IBPROF_CONF_SINGLE_THREAD) && (_IBPROF_CONF_SINGLE_THREAD == TRUE)
	#define CRITICAL_SECTION        int
	#define INIT_CRITICAL(x)        ((void)0)
	#define DELETE_CRITICAL(x)      ((void)0)
	#define ENTER_CRITICAL(x)       ((void)0)
	#define LEAVE_CRITICAL(x)       ((void)0)
#else
	#if defined(__LINUX__)
	#define CRITICAL_SECTION        pthread_mutex_t
	#if defined(_GNU_SOURCE)
		#define INIT_CRITICAL(x)    \
				{ \
					pthread_mutexattr_t attr; \
					pthread_mutexattr_init(&attr); \
					pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE); \
					pthread_mutex_init(x, &attr); \
					pthread_mutexattr_destroy(&attr); \
				}
	#else
		#define INIT_CRITICAL(x)    pthread_mutex_init(x, NULL)
	#endif
	#define DELETE_CRITICAL(x)      pthread_mutex_destroy(x)
	#define DBG_ENTER_CRITICAL(x)   pthread_mutex_lock(x)
	#define DBG_LEAVE_CRITICAL(x)   pthread_mutex_unlock(x)
#else
	#define CRITICAL_SECTION        CRITICAL_SECTION
	#define INIT_CRITICAL(x)        InitializeCriticalSection(x)
	#define DELETE_CRITICAL(x)      DeleteCriticalSection(x)
	#define DBG_ENTER_CRITICAL(x)   EnterCriticalSection(x)
	#define DBG_LEAVE_CRITICAL(x)   LeaveCriticalSection(x)
#endif

/* Debugging Multi-threading operations. */
#if defined(_DEBUG)
	#define ENTER_CRITICAL(x)   \
						{ \
						IBPROF_TRACE("lock = 0x%" PRIXPTR " <%s: %s #%d>\n", (uintptr_t)x, __FILE__, __FUNCTION__, __LINE__); \
						DBG_ENTER_CRITICAL(x); \
						}
	#define LEAVE_CRITICAL(x)   \
						{ \
						IBPROF_TRACE("unlock = 0x%" PRIXPTR " <%s: %s #%d>\n", (uintptr_t)x, __FILE__, __FUNCTION__, __LINE__); \
						DBG_LEAVE_CRITICAL(x); \
						}
#else
	#define ENTER_CRITICAL(x)       DBG_ENTER_CRITICAL(x)
	#define LEAVE_CRITICAL(x)       DBG_LEAVE_CRITICAL(x)
#endif /* _DEBUG */
#endif /* _IBPROF_CONF_SINGLE_THREAD */
/** @} */

/**
 * @name File
 * @brief File operations.
 */
/** @{ */
#if defined(__KERNEL__)
	#define     stdin       ((void *)1)
	#define     stdout      ((void *)1)
	#define     stderr      ((void *)1)
	#define     BUFSIZ      512
	#define sys_fopen(stream, mode)             (stdout)
	#define sys_freopen(src, mode, stream)      (stdout)
	#define sys_fclose(stream)                  ((void)0)
	#define sys_fsetbuf(stream, buf)            ((void)0)
	#define sys_fflush(stream)                  ((void)0)
	#define sys_frename(src, dest)              ((void)0)
	#define sys_fremove(src)                    ((void)0)
	#define sys_fstat(src, statbuf)             (-1)
	#define sys_fgetc(stream)                   ((int)0)
	#define sys_fgets(str, len, stream)         ((int)0)
	#define sys_feof(stream)                    ((int)0)
	#define sys_fread(buf, sz, count, stream)   ((void)0)
#else
	#define sys_fopen(stream, mode)             fopen(stream, mode)
	#define sys_freopen(src, mode, stream)      freopen(src, mode, stream)
	#define sys_fclose(stream)                  fclose(stream)
	#define sys_fsetbuf(stream, buf)            setbuf(stream, buf)
	#define sys_fflush(stream)                  fflush(stream)
	#define sys_frename(src, dest)              rename(src, dest)
	#define sys_fremove(src)                    remove(src)
	#define sys_fstat(src, statbuf)             stat(src, statbuf)
	#define sys_fgetc(stream)                   fgetc(stream)
	#define sys_fgets(str, len, stream)         fgets(str, len, stream)
	#define sys_feof(stream)                    feof(stream)
	#define sys_fread(buf, sz, count, stream)   fread(buf, sz, count, stream)
#endif
/** @} */

void *sys_dlsym(const char *symname, const char *symver);
int sys_dlcheck(const char *libname);

/**
 * sys_hexdump
 *
 * @brief
 *    Display buffer of defined length .
 *
 * @param[in]   *ptr            This is a pointer to the buffer to be displayed.
 * @param[in]    size           This is a size of the buffer in bytes.
 *
 * @retval @a none
 ***************************************************************************/
void sys_hexdump(void *ptr, int size);

/**
 * sys_fprintf
 *
 * @brief
 *    Sends a string to the output.
 *
 * @param[in]    stream         Pointer to the stream that specifies an output stream.
 * @param[in]    format         Specifies a pointer to the format string to print, as in printf.
 *                              The Format string supports the printf-style formatting codes..
 * @param[in]    ...            The arguments for format.
 *
 * @retval zero - on success
 * @retval non-zero - on failure
 ***************************************************************************/
int sys_fprintf(void *stream, const char *format, ...);

/**
 * sys_asprintf
 *
 * @brief
 *    Alocates output buffer and sends string to it.
 *
 * @param[in]    stream         Defines output buffer pointer.
 * @param[in]    format         Specifies a pointer to the format string to print, as in printf.
 *                              The Format string supports the printf-style formatting codes..
 * @param[in]    ...            The arguments for format.
 *
 * @retval positive - on success
 * @retval negative - on failure
 ***************************************************************************/
int sys_asprintf(char **stream, const char *format, ...);

/**
 * sys_malloc
 *
 * @brief
 *    Allocates memory for new category object and set initial values.
 *
 * @param[in]    size           This is a size of memory to allocate.
 *
 * @retval pointer to the allocated memory - on success
 * @retval NULL - on failure
 ***************************************************************************/
void *sys_malloc(size_t size);

/**
 * sys_free
 *
 * @brief
 *    This function releases allocated by log_malloc memory.
 *
 * @param[in]    *mem          This is a pointer to the memory to free.
 *
 * @retval @a none
 ***************************************************************************/
void sys_free(void *mem);

/**
 * sys_strdup
 *
 * @brief
 *    This function returns a newly allocated copy of string.
 *
 * @param[in]    string         This is a pointer to the original string.
 *
 * @retval pointer to new string - on success
 * @retval NULL - on failure
 ***************************************************************************/
char *sys_strdup(const char *string);

/**
 * sys_mode_check_env
 *
 * @brief
 *    This function treats input as format string
 *    USE_IBV=%d,USE_MXM=%d,USE_HCOL=%d
 *    in arbitrary order and puts results to corresponding variables.
 *
 * @param[in]    string         This is a pointer to the original string.
 *
 * @retval none
 ***************************************************************************/
void sys_mode_check_env(char *env);

/**
 * sys_time
 *
 * @brief
 *    Gives the number of seconds and microseconds since the  Epoch.
 *
 * @param[in]    tv             Argument is a struct timeval.
 *
 * @retval ( 0) - on success
 * @retval (-1) - on failure
 ***************************************************************************/
static int INLINE sys_time(struct timeval *tv)
{
	int status = 0;

#if defined(__LINUX__)
	status = gettimeofday(tv, NULL);
#else
	clock_t t = clock();
	tv->tv_sec = t / CLOCKS_PER_SEC;
	tv->tv_usec = t % CLOCKS_PER_SEC;
#endif

	return status;
}

/**
 * sys_rdtsc
 *
 * @brief
 *    Gives The Time Stamp Counter is a 64-bit register present on all
 *    x86 processors since the Pentium. It counts the number of ticks since reset.
 *    It is platform specific function.
 *
 * @retval (value) - on success
 * @retval ( 0) - on failure
 ***************************************************************************/
static unsigned long long INLINE sys_rdtsc(void)
{
    unsigned long long int result=0;

#if defined(__LINUX__)
    #if defined(__i386__)
        __asm volatile(".byte 0x0f, 0x31" : "=A" (result) : );

    #elif defined(__x86_64__)
        unsigned hi, lo;
        __asm volatile("rdtsc" : "=a"(lo), "=d"(hi));
        result = hi;
        result = result<<32;
        result = result|lo;

    #elif defined(__powerpc__)
        unsigned long int hi, lo, tmp;
        __asm volatile(
                    "0:                 \n\t"
                    "mftbu   %0         \n\t"
                    "mftb    %1         \n\t"
                    "mftbu   %2         \n\t"
                    "cmpw    %2,%0      \n\t"
                    "bne     0b         \n"
                    : "=r"(hi),"=r"(lo),"=r"(tmp)
                    );
        result = hi;
        result = result<<32;
        result = result|lo;

    #endif
#endif /* __LINUX__ */

    return (result);
}

/**
 * sys_hostdate
 *
 * @brief
 *    Return a string with host date. Should be freed with sys_free().
 *
 * @retval Host date - on success
 * @retval NULL - on failure
 ***************************************************************************/
char *sys_hostdate(void);

static INLINE char *sys_host()
{
	char name[256];
	char *host_name = NULL;
	char *domain = NULL;

	if (gethostname(name, sizeof(name)) != 0)
		sys_strcpy(name, "localhost");

	host_name = sys_strdup(name);

	domain = sys_strchr(host_name, '.');
	if (domain)
		*domain = '\0';

	return host_name;
}

static INLINE char *sys_user()
{
	char *str = NULL;

	str = getenv("USER");

	return sys_strdup(str ? str : "unknown");
}

static INLINE int sys_jobid()
{
	char *str = NULL;

	str = getenv("PBS_JOBID");			/* OpenPBS/Torque */
	str = (str ? str : getenv("LOADL_STEP_ID"));	/* LoadLeveler */
	str = (str ? str : getenv("SLURM_JOBID"));	/* SLURM */
	str = (str ? str : getenv("JOB_ID"));		/* SGE */
	str = (str ? str : getenv("LSB_JOBID"));	/* LSF */

	return sys_strtol((str ? str : "-1"), NULL, 10);
}

static INLINE int sys_procid()
{
	char *str = NULL;

	str = getenv("OMPI_COMM_WORLD_RANK");		/* OpenMPI */
	str = (str ? str : getenv("SLURM_PROCID"));
	str = (str ? str : getenv("LS_JOBPID"));

	return str ? sys_strtol(str, NULL, 10) : getpid();
}

static INLINE const char* sys_procname()
{
	const char *str = NULL;

	if (getenv("OMPI_COMM_WORLD_RANK"))
		str = "rank";
	else if (getenv("SLURM_PROCID") || getenv("LS_JOBPID"))
		str = "proc";
	else
		str = "pid";

	return str;
}

/**
 * sys_threadid
 *
 * @brief
 *    Returns the thread ID of the current process..
 *
 * @retval (value) - on success
 * @retval ( 0) - on failure
 ***************************************************************************/
static INLINE int sys_threadid(void)
{
	int tid = 0;

#if defined(__LINUX__)
	#include <sys/syscall.h>
	tid = syscall(__NR_gettid);
#else
	tid = GetCurrentThreadId();
#endif
	return tid;
}


#ifdef __cplusplus
}
#endif

#endif /* _IBPROF_DEF_H_ */

