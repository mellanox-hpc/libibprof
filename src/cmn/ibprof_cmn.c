/*
 * Copyright (c) 2013-2015 Mellanox Technologies, Inc.
 *                         All rights reserved.
 * $COPYRIGHT$
 *
 * Additional copyrights may follow
 *
 * $HEADER$
 */

#include "ibprof_cmn.h"

FILE *ibprof_dump_file;

/* This code is a workaround to RTLD_NEXT unexpected behaivour */
static const char *_libname = NULL;
static int _libname_counter = 0;


void *sys_dlsym(const char *symname, const char *symver)
{
	void *symaddr = NULL;
        const char *err = NULL;
#if defined(__LINUX__)
	void *libc_handle = RTLD_NEXT;
again:
	dlerror(); /* Clear any existing error */
	if (NULL == symver)
	symaddr = dlsym(libc_handle, symname);
	else
	symaddr = dlvsym(libc_handle, symname, symver);

	err = dlerror();
	if (!symaddr || err) {
		/* This code is a workaround to RTLD_NEXT unexpected behaivour */
		if (libc_handle == RTLD_NEXT) {
#ifndef __COVERITY__
			/* If the same library is loaded again with dlopen(), the same library
			 * handle is returned
			 */
			dlerror();    /* Clear any existing error */
			libc_handle = dlopen(_libname, RTLD_LAZY);
			err = dlerror();
			if (libc_handle && !err) {
				if (_libname_counter > 0) {
					dlclose(libc_handle);
				}
				_libname_counter++;
				goto again;
			}
#endif /* __COVERITY__ */
		}
		IBPROF_TRACE("Can't resolve %s: %s\n", symname, err);
	}
#else
#endif

	return symaddr;
}

int sys_dlcheck(const char *libname)
{
	int ret = IBPROF_ERR_NONE;
	void *libc_handle = NULL;
	const char *err = NULL;
#if defined(__LINUX__)
	dlerror();    /* Clear any existing error */
	libc_handle = dlopen(libname, RTLD_LAZY);

	err = dlerror();
	if (!libc_handle || err) {
		IBPROF_WARN("Can't find %s: %s\n", libname, err);
		ret = IBPROF_ERR_NOT_EXIST;
	}

	if (libc_handle) {
                _libname = libname;
                _libname_counter = 0;
		dlclose(libc_handle);
	}
#else
#endif

	return ret;
}

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
void sys_hexdump(void *ptr, int buflen)
{
	unsigned char *buf = (unsigned char *) ptr;
	char out_buf[120];
	int ret = 0;
	int out_pos = 0;
	int i, j;

	IBPROF_TRACE("dump data at %p\n", ptr);
	for (i = 0; i < buflen; i += 16) {
		out_pos = 0;
		ret = sprintf(out_buf + out_pos, "%06x: ", i);
		if (ret < 0)
			return;
		out_pos += ret;
		for (j = 0; j < 16; j++) {
			if (i + j < buflen)
				ret = sprintf(out_buf + out_pos, "%02x ",
						buf[i + j]);
			else
				ret = sprintf(out_buf + out_pos, "   ");
			if (ret < 0)
				return;
			out_pos += ret;
		}
		ret = sprintf(out_buf + out_pos, " ");
		if (ret < 0)
			return;
		out_pos += ret;
		for (j = 0; j < 16; j++)
			if (i + j < buflen) {
				ret = sprintf(out_buf + out_pos, "%c",
						isprint(buf[i+j]) ?
								buf[i + j] :
								'.');
				if (ret < 0)
					return;
				out_pos += ret;
			}
		ret = sprintf(out_buf + out_pos, "\n");
		if (ret < 0)
			return;
		IBPROF_TRACE("%s", out_buf);
	}
}

/**
 * sys_fprintf
 *
 * @brief
 *    Sends a string to the output.
 *
 * @param[in]    stream         Defines stream.
 * @param[in]    format         Specifies a pointer to the format string to print, as in printf.
 *                              The Format string supports the printf-style formatting codes..
 * @param[in]    ...            The arguments for format.
 *
 * @retval zero - on success
 * @retval non-zero - on failure
 ***************************************************************************/
int sys_fprintf(void *stream, const char *format, ...)
{
	int status = 0;
	va_list va;

	va_start(va, format);

	vfprintf((FILE *) stream, format, va);

	va_end(va);

	return status;
}

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

int sys_asprintf(char **stream, const char *format, ...)
{
	int ret = 0;
	va_list va;

	va_start(va, format);
	ret = vsnprintf(NULL, 0, format, va);
	va_end(va);

	if (ret > 0) {
		char* buffer = sys_malloc(ret + 1);
		if (buffer == NULL)
			return -1;

		va_start(va, format);
		ret = vsnprintf(buffer, ret + 1, format, va);
		va_end(va);

		sys_free((*stream));
		*stream = buffer;
	}

	return ret;
}

/**
 * sys_malloc
 *
 * @brief
 *    Allocate memory for new category object and set initial values.
 *
 * @param[in]    size           This is a size of memory to allocate.
 *
 * @retval pointer to the allocated memory - on success
 * @retval NULL - on failure
 ***************************************************************************/
void *sys_malloc(size_t size)
{
	void *p = NULL;

	p = malloc(size);
	if (p)
		sys_memset(p, 0, size);

	return p;
}

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
void sys_free(void *mem)
{
	if (mem)
		free(mem);
}

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
char *sys_strdup(const char *string)
{
	return string ?
		sys_strcpy((char *)sys_malloc(sys_strlen(string) + 1), string) :
		NULL;
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
char *sys_hostdate(void)
{
	char tmp_buf[256];
	char* host_date = NULL;
	time_t t;
	struct tm *tmp;

	sys_memset(tmp_buf, 0, sizeof(tmp_buf));

	t = time(NULL);
	tmp = localtime(&t);
	sys_snprintf_safe( tmp_buf, sizeof(tmp_buf), "%02d.%02d.%04d %02d:%02d:%02d",
			tmp->tm_mday, tmp->tm_mon + 1, tmp->tm_year + 1900,
			tmp->tm_hour, tmp->tm_min, tmp->tm_sec);

	host_date = sys_strdup(tmp_buf);

	return host_date;
}


