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

#define XML(tag_name, inner) "<" tag_name ">" inner "</" tag_name ">"

static int _ibprof_banner_dump(char **root, IBPROF_OBJECT *task_obj);

static int _ibprof_task_dump(char **root, IBPROF_TASK_OBJECT *task_obj);

static int _ibprof_module_dump(char **root, IBPROF_MODULE_OBJECT *module_obj, IBPROF_HASH_OBJECT* hash_obj, IBPROF_TASK_OBJECT* task_obj);

/**
 * ibprof_xml_dump
 *
 * @brief
 *    Dumps all gathered information in xml format.
 *
 * @return @a none
 ***************************************************************************/
void ibprof_io_xml_dump(FILE* file, IBPROF_OBJECT *ibprof_obj)
{
	char *buffer = NULL;
	char *banner = NULL;
	char *modules = NULL;
	char *module = NULL;
	int ret = 0;

	IBPROF_MODULE_OBJECT *temp_module_obj = NULL;
	int i = 0;

	ret = _ibprof_banner_dump(&banner, ibprof_obj);

	temp_module_obj = ibprof_obj->module_array[0];

	while (temp_module_obj) {
		if (temp_module_obj->id != IBPROF_MODULE_INVALID) {
			if (ibprof_hash_module_is_empty(temp_module_obj->id, ibprof_obj->hash_obj)) {
				temp_module_obj = ibprof_obj->module_array[++i];
				continue;
			}

			ret = _ibprof_module_dump(&module, temp_module_obj,
				ibprof_obj->hash_obj, ibprof_obj->task_obj);

			if (ret > 0) {
				ret = sys_asprintf(&modules,
					"%s%s",
					modules == NULL ? "" : modules,
					module);
			}
		}
		temp_module_obj = ibprof_obj->module_array[++i];
	}

	if (ret > 0) {
		ret = sys_asprintf(&buffer,
			XML("root",
				"%s" \
				XML("modules", "%s"))"\n",
			banner,
			modules);
	}

	sys_fprintf(file, "%s", buffer == NULL ? "" : buffer);

	sys_free(buffer);
	sys_free(banner);
	sys_free(modules);
	sys_free(module);
}

static int _ibprof_banner_dump(char **root, IBPROF_OBJECT *ibprof_obj)
{
	char *task_dump = NULL;
	int ret = 0;

	ret = _ibprof_task_dump(&task_dump, ibprof_obj->task_obj);

	if (ret > 0) {
		ret = sys_asprintf(root,
			XML("banner",
				XML("module",
					XML("name", "%s") \
					XML("version", "%s") \
					XML("compiled_date", "%s") \
					XML("compiled_time", "%s") \
					XML("copyright", "%s") \
					XML("task", "%s") \
					XML("warmup_number", "%d") \
					XML("Output time unit", "%s")
				)
			),
			__MODULE_NAME,
			STR(__MODULE_VERSION),
			__DATE__,
			__TIME__,
			__MODULE_COPYRIGHT,
			task_dump,
			ibprof_conf_get_int(IBPROF_WARMUP_NUMBER),
			ibprof_time_units_str[ibprof_conf_get_int(IBPROF_TIME_UNITS)]);
	}

	sys_free(task_dump);

	return ret;
}

static int _ibprof_task_dump(char **root, IBPROF_TASK_OBJECT *task_obj)
{
	int ret = 0;

	ret = sys_asprintf(root,
		XML("task",
			XML("date", "%s") \
			XML("host", "%s") \
			XML("user", "%s") \
			XML("jobid", "%d") \
			XML("rank", "%d") \
			XML("pid", "%d") \
			XML("tid", "%d") \
			XML("wall_time_in_sec", "%.2f") \
			XML("command_line", "%s") \
			XML("path", "%s") \
			XML("warm_up_number", "%d")),
		task_obj->date,
		task_obj->host,
		task_obj->user,
		task_obj->jobid,
		task_obj->procid,
		task_obj->pid,
		task_obj->tid,
		task_obj->wall_time,
		task_obj->cmdline,
		task_obj->cmdpath,
		ibprof_conf_get_int(IBPROF_WARMUP_NUMBER));

	return ret;
}

static const char *_ibprof_hash_format_xml(int module, const char* call_name, const char* stats_fmt, ...)
{
	static char buffer[1024];
	static char stat_buffer[1024];
	char *dest = stat_buffer;
	int dest_len = 0;
	va_list stats;
	int ret = 0;

	va_start(stats, stats_fmt);

	buffer[0] = '\0';

	ret = sys_vsnprintf(stat_buffer,
		sizeof(buffer),
		XML("count", "%ld") \
		XML("total", "%.4f") \
		XML("avg", "%.4f") \
		XML("max", "%.4f") \
		XML("min", "%.4f") \
		XML("fail", "%ld"),
		stats);
	if (ret >= 0)
		dest_len += ret;

	if (call_name && call_name[0]) {
		dest = buffer;

		ret = sys_snprintf_safe(buffer,
			sizeof(buffer) - dest_len,
			XML("call",
				XML("name", "%s") \
				"%s"),
			call_name,
			stat_buffer);
	}

	va_end(stats);

	return (ret > 0 ? dest : NULL);
}

static int _ibprof_module_dump(char **module, IBPROF_MODULE_OBJECT *module_obj, IBPROF_HASH_OBJECT* hash_obj, IBPROF_TASK_OBJECT *task_obj)
{
	const IBPROF_MODULE_CALL *temp_module_call = NULL;
	char *str = NULL;
	double total_time = 0;
	char *module_calls = NULL;
	int ret = 0;

	if (module_obj->tbl_call) {
		char *module_call = NULL;
		temp_module_call = module_obj->tbl_call;

		while (temp_module_call && (temp_module_call->call != UNDEFINED_VALUE && temp_module_call->name))
		{
			str = ibprof_hash_dump(
				hash_obj,
				module_obj->id,
				temp_module_call->call,
				task_obj->procid,
				_ibprof_hash_format_xml);

			if (str && str[0]) {
				ret = sys_asprintf(&module_call,
					XML("call",
						XML("name", "%s") \
						"%s"),
					temp_module_call->name ? temp_module_call->name : "unknown", str);
				if (ret > 0) {
					ret = sys_asprintf(&module_calls, "%s%s",
						module_calls == NULL ? "" : module_calls,
						module_call);
				}
			}

			free(str);
			temp_module_call++;
		}
		sys_free(module_call);

	} else if (module_obj->id == IBPROF_MODULE_USER) {
			str = ibprof_hash_dump(
					hash_obj,
					module_obj->id,
					UNDEFINED_VALUE,
					task_obj->procid,
					_ibprof_hash_format_xml);
			if (str && str[0]) {
				ret = sys_asprintf(&module_calls, "%s", str);
			}
			free(str);
	}

	total_time = ibprof_hash_module_total(hash_obj,
		module_obj->id,
		task_obj->procid);

	ret = sys_asprintf(module,
		XML("module",
			XML("name", "%s") \
			XML("calls","%s") \
			XML("total", "%.4f") \
			XML("wall_time_percent", "%.4f")),
			module_obj->name ? module_obj->name : "unknown",
			module_calls,
			total_time,
			total_time / (task_obj->wall_time * 1.0e+6));

	sys_free(module_calls);

	return ret;
}

