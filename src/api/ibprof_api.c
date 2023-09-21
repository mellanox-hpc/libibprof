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
#include "ibprof_api.h"
#include "ibprof_types.h"

#include "ibprof_io.h"

/*
 * List of supported modules
 */
extern IBPROF_MODULE_OBJECT ibv_module;
extern IBPROF_MODULE_OBJECT hcol_module;
extern IBPROF_MODULE_OBJECT mxm_module;
extern IBPROF_MODULE_OBJECT pmix_module;
extern IBPROF_MODULE_OBJECT shmem_module;

/****************************************************************************
 * Configuration options
 ***************************************************************************/
#ifndef CONF_TIMESTAMP
#define CONF_TIMESTAMP  0
#endif

typedef void (*ibprof_format_dump)(FILE*, IBPROF_OBJECT*);

ibprof_format_dump format_dump;

/****************************************************************************
 * Static Function Declarations
 ***************************************************************************/
static IBPROF_ERROR __get_env(void);
#if defined(CONF_TIMESTAMP) && (CONF_TIMESTAMP == 1)
static double __get_cpu_clocks_per_sec(void);
#endif /* CONF_TIMESTAMP */

static IBPROF_MODULE_OBJECT user_module = {IBPROF_MODULE_USER,
					       "user",
					       NULL,
					       NULL,
					       NULL,
					       NULL,
					       NULL
};
static IBPROF_MODULE_OBJECT *__ibprof_modules[] = {
	&ibv_module,
	&hcol_module,
	&mxm_module,
	&pmix_module,
	&shmem_module,
	&user_module,
	NULL
};

/*
 * It is the core of profiling facility
 */
void __ibprof_init(void);
void __ibprof_exit(void);

static IBPROF_OBJECT *ibprof_obj = NULL;	/* Verify a pointer to this object with NULL to check ACTIVE/CLOSE */
pthread_once_t ibprof_initialized = PTHREAD_ONCE_INIT;

#if defined(HAVE_VISIBILITY)
#pragma GCC visibility push(default)
#endif

double ibprof_timestamp(void)
{
#if defined(CONF_TIMESTAMP) && (CONF_TIMESTAMP == 1)
	return ((double)sys_rdtsc() / __get_cpu_clocks_per_sec());
#else
	struct timeval tv;

	sys_time(&tv);

	return ((tv.tv_sec) + (tv.tv_usec) * 1.0e-6);
#endif
}

void ibprof_update(int module, int call, double tm)
{
	IBPROF_HASH_OBJ *entry = NULL;
	HASH_KEY key;

	if (ibprof_obj) {
		key = HASH_KEY_SET(module, call, ibprof_obj->task_obj->procid, 0);

		entry = ibprof_hash_find(ibprof_obj->hash_obj, key);
		if (entry)
			ibprof_hash_update(ibprof_obj->hash_obj, entry, tm);
	}
}

void ibprof_update_ex(int module, int call, double tm, void *ctx)
{
	IBPROF_HASH_OBJ *entry = NULL;
	HASH_KEY key;

	if (ibprof_obj) {
		key = HASH_KEY_SET(module, call, ibprof_obj->task_obj->procid, 0);

		entry = ibprof_hash_find(ibprof_obj->hash_obj, key);
		if (entry)
			ibprof_hash_update_ex(ibprof_obj->hash_obj, entry, tm, ctx);
	}
}


void ibprof_interval_start(int callid, const char* name)
{
	IBPROF_HASH_OBJ *entry = NULL;
	HASH_KEY key;

	if (ibprof_obj) {
		key = HASH_KEY_SET(IBPROF_MODULE_USER, callid, ibprof_obj->task_obj->procid, 0);

		entry = ibprof_hash_find(ibprof_obj->hash_obj, key);
		if (entry && (entry->t_start < 0)){
			if (!entry->call_name[0])
				strncpy(entry->call_name, name, sizeof(entry->call_name) - 1);
			entry->t_start = ibprof_timestamp();
		}
	}
}

void ibprof_interval_end(int callid)
{
	IBPROF_HASH_OBJ *entry = NULL;
	HASH_KEY key;

	if (ibprof_obj) {
		key = HASH_KEY_SET(IBPROF_MODULE_USER, callid, ibprof_obj->task_obj->procid, 0);

		entry = ibprof_hash_find(ibprof_obj->hash_obj, key);
		if (entry && (entry->t_start >= 0)){
			ibprof_hash_update(ibprof_obj->hash_obj, entry,
						ibprof_timestamp_diff(entry->t_start));
			entry->t_start = UNDEFINED_VALUE;
		}
	}
}

void ibprof_dump(void)
{
	if (ibprof_obj && ibprof_hash_count(ibprof_obj->hash_obj)) {
		format_dump(ibprof_dump_file, ibprof_obj);
		/* Cleanup hash after dump */
		ENTER_CRITICAL(&(ibprof_obj->lock));
		ibprof_hash_destroy(ibprof_obj->hash_obj);
		ibprof_obj->hash_obj = ibprof_hash_create();
		LEAVE_CRITICAL(&(ibprof_obj->lock));
	}
}

#if defined(HAVE_VISIBILITY)
#pragma GCC visibility pop
#endif

/****************************************************************************
 * Static Function Declarations
 ***************************************************************************/
static IBPROF_ERROR __get_env(void)
{
	IBPROF_ERROR status = IBPROF_ERR_NONE;
	char *env;

	ibprof_conf_init();

	env = ibprof_conf_get_string(IBPROF_DUMP_FILE);
	if (env) {
		ibprof_dump_file = sys_fopen(env, "a+");
		if (ibprof_dump_file == NULL ) {
			status = IBPROF_ERR_NOT_EXIST;
	                IBPROF_FATAL("%s : error=%d - Can't create a dump file '%s'\n",
	                		__FUNCTION__, status, env);
		} else
			setvbuf(ibprof_dump_file, NULL, _IOLBF, 1024);
	}

	format_dump = ibprof_io_plain_dump;

	env = ibprof_conf_get_string(IBPROF_FORMAT);
	if (env) {
		if (sys_strcasecmp(env, "xml") == 0)
			format_dump = ibprof_io_xml_dump;
	}

	return status;
}

#if defined(CONF_TIMESTAMP) && (CONF_TIMESTAMP == 1)
static double __get_cpu_clocks_per_sec(void)
{
	static double clocks_per_sec = 0.0;
	static int initialized = 0;

	if (!initialized) {
	        double mhz = 0.0;
#if defined(__LINUX__)
		FILE* f;
		char buf[256];

		f = fopen("/proc/cpuinfo", "r");
		if (!f) {
			return 0.0;
		}

		while (fgets(buf, sizeof(buf), f)) {
			double m;
			int rc;

#if defined(__ia64__)
			rc = sscanf(buf, "itc MHz : %lf", &m);
#elif defined(__powerpc__)
			rc = sscanf(buf, "clock : %lf", &m);
#else
			rc = sscanf(buf, "cpu MHz : %lf", &m);
#endif
			if (rc != 1) {
				continue;
			}
			if (mhz == 0.0) {
				mhz = m;
				continue;
			}
			if (mhz != m) {
				double mm = sys_max(mhz, m);
				mhz = mm;
			}
		}
		fclose(f);
#endif
	        clocks_per_sec = mhz * 1.0e6;
	        initialized = 1;
	}

	return clocks_per_sec;
}
#endif /* CONF_TIMESTAMP */

/****************************************************************************
 * Load/unload open/exit
 ***************************************************************************/


/*
 * Shared object initializer.
 * This function is called before dlopen() returns.
 */

void __attribute__((constructor)) __ibprof_init(void)
{
	IBPROF_ERROR status = IBPROF_ERR_NONE;

	ibprof_dump_file = (FILE *)stderr;

	/* check if it has been activated */
	if (!ibprof_obj) {
		IBPROF_OBJECT *temp_ibprof_obj = NULL;
		IBPROF_MODULE_OBJECT *temp_module_obj = NULL;
		int i = 0;

		/* allocate memory for basis object */
		temp_ibprof_obj = (IBPROF_OBJECT *) sys_malloc(sizeof(IBPROF_OBJECT));
		if (!temp_ibprof_obj) {
			status = IBPROF_ERR_NO_MEMORY;
		        IBPROF_FATAL("%s : error=%d - Can't allocate memory\n",
		        		__FUNCTION__, status);
		}

		/* get/proc environment variables */
		if (status == IBPROF_ERR_NONE) {
			status = __get_env();
		}

		/* initialize available modules */
		if (status == IBPROF_ERR_NONE) {
			temp_ibprof_obj->module_array = __ibprof_modules;

			i = 0;
			temp_module_obj = temp_ibprof_obj->module_array[0];
			while (!status && temp_module_obj) {
				if (temp_module_obj->id	!= IBPROF_MODULE_INVALID) {
					if (temp_module_obj->init) {
						status = temp_module_obj->init(temp_module_obj);

						/* Mark module as invalid in case all or some
						 * symbols can not be resolved
						 */
						if (status == IBPROF_ERR_NOT_EXIST || status == IBPROF_ERR_UNSUPPORTED) {
							IBPROF_WARN("Can't resolve symbols from %s module\n",
									temp_module_obj->name);
							temp_module_obj->id = IBPROF_MODULE_INVALID;
							status = IBPROF_ERR_NONE;
						}
					}
				}
				temp_module_obj = temp_ibprof_obj->module_array[++i];
			}
		}

		/* initialize hash object */
		if (status == IBPROF_ERR_NONE) {
			temp_ibprof_obj->hash_obj = ibprof_hash_create();
			if (!temp_ibprof_obj->hash_obj) {
				status = IBPROF_ERR_INCORRECT;
		                IBPROF_FATAL("%s : error=%d - Can't create hash object\n",
		                		__FUNCTION__, status);
			}
		}

		/* initialize task object */
		if (status == IBPROF_ERR_NONE) {
			temp_ibprof_obj->task_obj = ibprof_task_create();
			if (!temp_ibprof_obj->task_obj) {
				status = IBPROF_ERR_INCORRECT;
	                        IBPROF_FATAL("%s : error=%d - Can't create hash object\n",
	                                __FUNCTION__, status);
			}
		}

		if (status == IBPROF_ERR_NONE) {
			INIT_CRITICAL(&(temp_ibprof_obj->lock));

			ENTER_CRITICAL(&(temp_ibprof_obj->lock));

			ibprof_obj = temp_ibprof_obj;

			LEAVE_CRITICAL(&(ibprof_obj->lock));
		}

		if (status != IBPROF_ERR_NONE) {
			if (temp_ibprof_obj) {
				if (temp_ibprof_obj->module_array) {
					IBPROF_MODULE_OBJECT *temp_module_obj = NULL;
					int i = 0;

					temp_module_obj = temp_ibprof_obj->module_array[0];
					while (temp_module_obj) {
						if (temp_module_obj->id != IBPROF_MODULE_INVALID) {
							if (temp_module_obj->exit)
								status = temp_module_obj->exit(temp_module_obj);
						}
						temp_module_obj = temp_ibprof_obj->module_array[++i];
					}
				}

				if (temp_ibprof_obj->hash_obj)
					ibprof_hash_destroy(temp_ibprof_obj->hash_obj);

				if (temp_ibprof_obj->task_obj)
					ibprof_task_destroy(temp_ibprof_obj->task_obj);

				sys_free(temp_ibprof_obj);
			}
		}
	}

	if (status != IBPROF_ERR_NONE) {
                IBPROF_FATAL("%s : error=%d - Can't load %s\n",
                        __FUNCTION__, status, __MODULE_NAME);
		exit(EXIT_FAILURE);
	}
}

void __attribute__((destructor)) __ibprof_exit(void)
{
	IBPROF_ERROR status = IBPROF_ERR_NONE;
	UNREFERENCED_PARAMETER(status);

	/* check if it has been activated */
	if (ibprof_obj) {
		IBPROF_MODULE_OBJECT *temp_module_obj = NULL;
		int i = 0;

		ibprof_obj->task_obj->wall_time =
			ibprof_task_wall_time(ibprof_obj->task_obj->t_start);

		/* Dump all gathered information */
		ibprof_dump();

		temp_module_obj = ibprof_obj->module_array[0];
		while (temp_module_obj) {
			if (temp_module_obj->id != IBPROF_MODULE_INVALID) {
				if (temp_module_obj->exit)
					status = temp_module_obj->exit(temp_module_obj);
			}
			temp_module_obj = ibprof_obj->module_array[++i];
		}

		ibprof_hash_destroy(ibprof_obj->hash_obj);

		ibprof_task_destroy(ibprof_obj->task_obj);

		DELETE_CRITICAL(&(ibprof_obj->lock));

		sys_free(ibprof_obj);
		ibprof_obj = NULL;
	}

	if (ibprof_dump_file &&
		ibprof_dump_file != stdout &&
		ibprof_dump_file != stderr) {

		struct stat     statbuf;
		char fd_path[255];
		char *filename = sys_malloc(255);
		size_t ret = 0;

		sys_sprintf(fd_path, "/proc/self/fd/%d", fileno(ibprof_dump_file));

		ret = readlink(fd_path, filename, 255);

		if (ret > 0) {
			sys_fflush(ibprof_dump_file);
			sys_fclose(ibprof_dump_file);
			if (!sys_fstat(filename, &statbuf))
				if (!statbuf.st_size)
					  ret = sys_fremove(filename);
		}
	        sys_free(filename);
	}

	return ;
}
