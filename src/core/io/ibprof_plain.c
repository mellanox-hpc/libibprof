/*
 * Copyright (c) 2013-2015 Mellanox Technologies, Inc.
 *                         All rights reserved.
 * $COPYRIGHT$
 *
 * Additional copyrights may follow
 *
 * $HEADER$
 */

#include "ibprof_types.h"
#include "ibprof_io.h"

typedef int (*ibprof_io_plain_output)(void* stream, const char *format, ...);

ibprof_io_plain_output plain_output = sys_fprintf;

static const char* DELIMITER="===============================================================================================\n";

static char *hostname = "";

static int pid = 0;

static void _ibprof_task_dump(IBPROF_TASK_OBJECT *task_obj);

static void _ibprof_banner_dump(FILE* file, IBPROF_OBJECT *ibprof_obj);

static void _ibprof_module_dump(FILE* file, IBPROF_MODULE_OBJECT *module_obj, IBPROF_HASH_OBJECT* hash_obj, int proc_id);

static int _ibprof_io_plain_prefix(void *stream, const char* format, ...);

/**
 * ibprof_plain_dump
 *
 * @brief
 *    Dumps all gathered information in plain.
 *
 * @return @a none
 ***************************************************************************/
void ibprof_io_plain_dump(FILE* file, IBPROF_OBJECT *ibprof_obj)
{
	IBPROF_MODULE_OBJECT *temp_module_obj = NULL;
	int i = 0;
	double total_time = 0;

	if (ibprof_conf_get_int(IBPROF_OUTPUT_PREFIX)) {
		plain_output = _ibprof_io_plain_prefix;
		hostname = ibprof_obj->task_obj->host;
		pid = ibprof_obj->task_obj->procid;
	}

	_ibprof_banner_dump(file, ibprof_obj);

	temp_module_obj = ibprof_obj->module_array[0];

	while (temp_module_obj) {
		if (temp_module_obj->id != IBPROF_MODULE_INVALID) {
			if (ibprof_hash_module_is_empty(temp_module_obj->id, ibprof_obj->hash_obj)) {
				temp_module_obj = ibprof_obj->module_array[++i];
				continue;
			}

			_ibprof_module_dump(file, temp_module_obj, ibprof_obj->hash_obj, ibprof_obj->task_obj->procid);

			total_time = ibprof_hash_module_total(ibprof_obj->hash_obj,
				temp_module_obj->id,
				ibprof_obj->task_obj->procid);

			plain_output(file, "%-30.30s :    %20.4f\n", "total", total_time);
			plain_output(file, DELIMITER);
			plain_output(file, "%-30.30s :    %20.4f %\n", "wall time (%)", total_time
				/ (ibprof_obj->task_obj->wall_time * 1.0e+6));
			plain_output(file, DELIMITER);
		}

		temp_module_obj = ibprof_obj->module_array[++i];
	}

	return;
}

static void _ibprof_banner_dump(FILE* file, IBPROF_OBJECT *ibprof_obj)
{

	plain_output(file, "\n");
	plain_output(file, DELIMITER);
	plain_output(file, __MODULE_NAME ", version %s\n",
		STR(__MODULE_VERSION));
	plain_output(file, "   compiled %s, %s\n\n",
		__DATE__,
		__TIME__);
	plain_output(file, "%s\n\n", __MODULE_COPYRIGHT);

	_ibprof_task_dump(ibprof_obj->task_obj);
	plain_output(file,"warmup number : %d\n", ibprof_conf_get_int(IBPROF_WARMUP_NUMBER));
	plain_output(file,"Output time units : %s\n", ibprof_time_units_str[ibprof_conf_get_int(IBPROF_TIME_UNITS)]);
	plain_output(file, DELIMITER);

	return;
}

static void _ibprof_task_dump(IBPROF_TASK_OBJECT *task_obj)
{
	if (task_obj) {
		plain_output(ibprof_dump_file, "date : %s\n", task_obj->date);
		plain_output(ibprof_dump_file, "host : %s\n", task_obj->host);
		plain_output(ibprof_dump_file, "user : %s\n", task_obj->user);
		plain_output(ibprof_dump_file, "jobid : %d\n", task_obj->jobid);
		plain_output(ibprof_dump_file, "%s : %d\n", "rank", task_obj->procid);
		plain_output(ibprof_dump_file, "pid : %d\n", task_obj->pid);
		plain_output(ibprof_dump_file, "tid : %d\n", task_obj->tid);
		plain_output(ibprof_dump_file, "wall time (sec) : %.2f\n", task_obj->wall_time);
		plain_output(ibprof_dump_file, "command line : %s\n", task_obj->cmdline);
		plain_output(ibprof_dump_file, "path : %s\n", task_obj->cmdpath);
	}

	return;
}

static const char *_ibprof_hash_format_plain(int module, const char* call_name, const char* stats_fmt, ...)
{
	static char buffer[1024];
	char *dest = buffer;
	int dest_len = 0;
	va_list stats;
	int ret = 0;

	va_start(stats, stats_fmt);

	buffer[0] = '\0';

	if (call_name && call_name[0]) {
		ret = sys_snprintf_safe((dest + dest_len),
			sizeof(buffer) - dest_len,
			"%-30.30s : ",
			call_name);
		if (ret >= 0)
			dest_len += ret;
	}

	switch (ibprof_conf_get_mode(module)) {
	case IBPROF_MODE_ERR:
		ret = sys_vsnprintf((dest + dest_len),
			sizeof(buffer) - dest_len,
			"%10ld   %10.4f   %10.4f   %10.4f   %10.4f   %10ld",
			stats);
		break;

	default:
		ret = sys_vsnprintf((dest + dest_len),
			sizeof(buffer) - dest_len,
			"%10ld   %10.4f   %10.4f   %10.4f   %10.4f",
			stats);
		break;
	}

	va_end(stats);

	return (ret > 0 ? buffer: NULL);
}

static void _ibprof_module_dump(FILE* file, IBPROF_MODULE_OBJECT *module_obj, IBPROF_HASH_OBJECT* hash_obj, int proc_id)
{
	const IBPROF_MODULE_CALL *temp_module_call = NULL;
	char *str = NULL;

	plain_output(file, "\n");
	switch (ibprof_conf_get_mode(module_obj->id)) {
	case IBPROF_MODE_ERR:
		plain_output(file, "%-30.30s : %10s   %10s   %10s   %10s   %10s   %10s\n",
			(module_obj->name ? module_obj->name : "unknown"), "count",
						"total", "avg", "max", "min", "fail");
		break;

	default:
		plain_output(file, "%-30.30s : %10s   %10s   %10s   %10s   %10s\n",
			(module_obj->name ? module_obj->name : "unknown"), "count",
						"total", "avg", "max", "min");
		break;
	}
	plain_output(file, DELIMITER);

	if (module_obj->tbl_call) {
		temp_module_call = module_obj->tbl_call;

		while (temp_module_call	&& (temp_module_call->call	!= UNDEFINED_VALUE &&
			temp_module_call->name)) {

			str = ibprof_hash_dump(hash_obj, module_obj->id,
				temp_module_call->call, proc_id,
				_ibprof_hash_format_plain);

			if (str && str[0]) {
				plain_output(file, "%-30.30s : %s\n",
					(temp_module_call->name ? temp_module_call->name : "unknown"), str);
			}

			sys_free(str);
			temp_module_call++;
		}
	} else if (module_obj->id == IBPROF_MODULE_USER) {
		str = ibprof_hash_dump(hash_obj, module_obj->id,
					UNDEFINED_VALUE, proc_id,
					_ibprof_hash_format_plain);

		if (str && str[0]) {
			plain_output(file, "%s", str);
		}
		sys_free(str);
	}
	plain_output(file, DELIMITER);

	return;
}

static int _ibprof_io_plain_prefix(void *stream, const char* format, ...)
{
	char *buffer, *ptr;
	int size = 0;
	va_list args;

	va_start(args, format);
	size  = sys_vsnprintf(NULL, 0, format, args);
	va_end(args);
	if (0 >= size)
		return -1;

	buffer = (char*) sys_malloc(size);
	if (NULL == buffer)
		return -1;

	va_start(args, format);
	sys_vsnprintf(buffer, size, format, args);
	va_end(args);

	/* We need to plainify output to make sure
	 * that each line prefixed with libibprof:host:pid
	 */
	ptr = sys_strstr(buffer, "\n");
	while (ptr != NULL) {
		*ptr = ' ';
		ptr = sys_strstr(buffer, "\n");
	}

	/* Skip empty lines */
	if (strcmp(buffer, ""))
		sys_fprintf(stream, "[libibprof:%s:%d] %s\n", hostname, pid, buffer);

	sys_free(buffer);

	return 0;
}
