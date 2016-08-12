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
#include "ibprof_conf.h"
#include "ibprof_api.h"

static void* enviroment[IBPROF_ENV_OPTIONS_AMOUNT];

static void _ibprof_conf_init(void);

void ibprof_conf_init(void)
{
#if defined(_DEBUG)
	static uint32_t ibprof_test_mask = IBPROF_LOG_FATAL | IBPROF_LOG_ERR | IBPROF_LOG_WARN | IBPROF_LOG_INFO | IBPROF_LOG_TRACE;
#else
	static uint32_t ibprof_test_mask = IBPROF_LOG_FATAL | IBPROF_LOG_ERR | IBPROF_LOG_WARN;
#endif
	static int ibprof_mode_ibv = IBPROF_MODE_PROF;
	static int ibprof_mode_hcol = IBPROF_MODE_PROF;
	static int ibprof_mode_mxm = IBPROF_MODE_PROF;
	static int ibprof_mode_pmix = IBPROF_MODE_PROF;
	static int ibprof_mode_shmem = IBPROF_MODE_PROF;
	static int ibprof_output_prefix = 0;
	static int ibprof_warmup_number = 0;
	static const char *ibprof_dump_file_name = NULL;
	static const char *ibprof_format = NULL;
	static int ibprof_err_percent = 1;
	static int ibprof_err_seed = 1337;

	enviroment[IBPROF_TEST_MASK] = (void *) &ibprof_test_mask;
	enviroment[IBPROF_MODE_IBV] = (void *) &ibprof_mode_ibv;
	enviroment[IBPROF_MODE_HCOL] = (void *) &ibprof_mode_hcol;
	enviroment[IBPROF_MODE_MXM] = (void *) &ibprof_mode_mxm;
	enviroment[IBPROF_MODE_PMIX] = (void *) &ibprof_mode_pmix;
	enviroment[IBPROF_MODE_SHMEM] = (void *) &ibprof_mode_shmem;
	enviroment[IBPROF_OUTPUT_PREFIX] = (void *) &ibprof_output_prefix;
	enviroment[IBPROF_WARMUP_NUMBER] = (void *) &ibprof_warmup_number;
	enviroment[IBPROF_DUMP_FILE] = (void *) ibprof_dump_file_name;
	enviroment[IBPROF_FORMAT] = (void *) ibprof_format;
	enviroment[IBPROF_ERR_PERCENT] = (void *) &ibprof_err_percent;
	enviroment[IBPROF_ERR_SEED] = (void *) &ibprof_err_seed;

	_ibprof_conf_init();
}

char* ibprof_conf_get_string(IBPROF_ENV variable)
{
	return (char *) enviroment[variable];
}

int ibprof_conf_get_int(IBPROF_ENV variable)
{
	return * (int*) enviroment[variable];
}

static void _ibprof_conf_mode(char *str);

static void _ibprof_conf_dump_file(char *str);

static void _ibprof_conf_init(void)
{
	char *env;
	env = getenv("IBPROF_MODE");
	if (env)
		_ibprof_conf_mode(env);

	env = getenv("IBPROF_TEST_MASK");
	if (env)
		*(uint32_t *) enviroment[IBPROF_TEST_MASK] = sys_strtol(env, NULL, 0);

	env = getenv("IBPROF_OUTPUT_PREFIX");
	if (env)
		*(int *) enviroment[IBPROF_OUTPUT_PREFIX] = sys_strtol(env, NULL, 0);

	env = getenv("IBPROF_WARMUP_NUMBER");
	if (env)
		*(int *) enviroment[IBPROF_WARMUP_NUMBER] = sys_strtol(env, NULL, 0);

	env = getenv("IBPROF_DUMP_FILE");
	if (env)
		_ibprof_conf_dump_file(env);

	env = getenv("IBPROF_FORMAT");
	if (env)
		enviroment[IBPROF_FORMAT] = (void *) env;

	env = getenv("IBPROF_ERR_PERCENT");
	if (env)
		*(int *) enviroment[IBPROF_ERR_PERCENT] = sys_strtol(env, NULL, 0);

	env = getenv("IBPROF_ERR_SEED");
	if (env) {
		*(int *) enviroment[IBPROF_ERR_SEED] = sys_strtol(env, NULL, 0);
		srand(*(int *) enviroment[IBPROF_ERR_SEED]);
	}
}

static void _ibprof_conf_mode(char *env)
{
	char *lower_env, *ptr;
	int i;

	i = sys_strlen(env);
	lower_env = sys_strdup(env);

	while (i--)
		lower_env[i] = tolower(env[i]); 

	ptr = sys_strstr(lower_env, "use_ibv");

	if (NULL != ptr) {
		sscanf(ptr, "use_ibv=%d", (int *) enviroment[IBPROF_MODE_IBV]);
	}

	ptr = sys_strstr(lower_env, "use_hcol");
	if (NULL != ptr) {
		sscanf(ptr, "use_hcol=%d", (int *) enviroment[IBPROF_MODE_HCOL]);
	}

	ptr = sys_strstr(lower_env, "use_mxm");
	if (NULL != ptr) {
		sscanf(ptr, "use_mxm=%d", (int *) enviroment[IBPROF_MODE_MXM]);
	}

	ptr = sys_strstr(lower_env, "use_pmix");
	if (NULL != ptr) {
		sscanf(ptr, "use_pmix=%d", (int *) enviroment[IBPROF_MODE_PMIX]);
	}

	ptr = sys_strstr(lower_env, "use_shmem");
	if (NULL != ptr) {
		sscanf(ptr, "use_shmem=%d", (int *) enviroment[IBPROF_MODE_SHMEM]);
	}

	sys_free(lower_env);
}

static void _ibprof_conf_dump_file(char *str)
{
	char *pattern = str;
	int max_len = 1024;
	char buf[1024];
	char *dest = buf;
	int dest_len = 0;
        char *tmp = NULL;

	while (pattern && ((max_len - dest_len) > 1)) {
		char c = *pattern++;
		int ret = 0;

		if (c != '%') {
			*(dest + dest_len) = c;
			dest_len++;
			if ('\0' == c)
				break;
			continue;
		}

		c = *pattern++;
		switch (c) {
		case 'J': /* add Job ID */
			ret =
					sys_snprintf_safe(
							(dest + dest_len),
							(max_len - dest_len),
							"%d", sys_jobid());
			break;

		case 'H': /* add Host name */
                        tmp = sys_host();
			ret =
					sys_snprintf_safe(
							(dest + dest_len),
							(max_len - dest_len),
							"%s", tmp);
			sys_free(tmp);
			break;

		case 'T': /* add Task ID */
			ret =
					sys_snprintf_safe(
							(dest + dest_len),
							(max_len - dest_len),
							"%d", sys_procid());
			break;
		}
		if (ret >= 0)
			dest_len += ret;
		else
			break; /* size of buffer is exceeded */
	}
	enviroment[IBPROF_DUMP_FILE] = (void *) &buf;
}

int ibprof_conf_get_mode(int module)
{
	int mode = IBPROF_MODE_NONE;

	/* Get current mode for the module */
	switch (module) {
	case IBPROF_MODULE_IBV:
		mode = ibprof_conf_get_int(IBPROF_MODE_IBV);
		break;

	case IBPROF_MODULE_HCOL:
		mode = ibprof_conf_get_int(IBPROF_MODE_HCOL);
		break;

	case IBPROF_MODULE_MXM:
		mode = ibprof_conf_get_int(IBPROF_MODE_MXM);
		break;

	case IBPROF_MODULE_PMIX:
		mode = ibprof_conf_get_int(IBPROF_MODE_PMIX);
		break;

	case IBPROF_MODULE_SHMEM:
		mode = ibprof_conf_get_int(IBPROF_MODE_SHMEM);
		break;

	default:
		mode = IBPROF_MODE_NONE;
	}

	return mode;
}
