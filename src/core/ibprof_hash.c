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
#include "ibprof_types.h"

#include "ibprof_hash.h"

#define TO_TIME(t_val) ((t_val) * 1000)

/****************************************************************************
 * Static Function Declarations
 ***************************************************************************/

/**
 * ibprof_hash_create
 *
 * @brief
 *    Allocates memory for new hash object and set initial values.
 *
 * @retval pointer to new hash object - on success
 * @retval NULL - on failure
 ***************************************************************************/
IBPROF_HASH_OBJECT *ibprof_hash_create(void)
{
	IBPROF_HASH_OBJECT *hash_obj = NULL;

	hash_obj = (IBPROF_HASH_OBJECT *) sys_malloc(sizeof(IBPROF_HASH_OBJECT));
	if (hash_obj) {
		hash_obj->size = HASH_MAX_SIZE;
		hash_obj->hash_table = (IBPROF_HASH_OBJ *) sys_malloc(
				hash_obj->size * sizeof(IBPROF_HASH_OBJ));
		if (hash_obj->hash_table) {
			int i = 0;

			sys_memset(hash_obj->hash_table,
					0,
					hash_obj->size * sizeof(IBPROF_HASH_OBJ));
			hash_obj->last = NULL;
			hash_obj->count = 0;
			for (i = 0; i < hash_obj->size; i++)
				hash_obj->hash_table[i].key = HASH_KEY_INVALID;
		} else {
			sys_free(hash_obj);
			hash_obj = NULL;
		}
	}

	return hash_obj;
}

/**
 * ibprof_hash_destroy
 *
 * @brief
 *    Releases all used resources and free memory allocated for internal object.
 *
 * @param[in]    hash_obj        Hash object.
 *
 * @return @a none
 ***************************************************************************/
void ibprof_hash_destroy(IBPROF_HASH_OBJECT *hash_obj)
{
	if (hash_obj) {
		sys_free(hash_obj->hash_table);
		sys_free(hash_obj);
	}
}


/**
 * ibprof_hash_module_total
 *
 * @brief
 *    Dump total collected time in ms for module.
 *
 * @return formatted string
 ***************************************************************************/
double ibprof_hash_module_total(IBPROF_HASH_OBJECT *hash_obj,
		int module,
		int rank)
{
	int i = 0;
	double result_total = 0;

	for (i = 0; i < hash_obj->size; i++) {
		if (hash_obj->hash_table[i].key != HASH_KEY_INVALID) {
			if (module != HASH_KEY_GET_MODULE(hash_obj->hash_table[i].key))
				continue;
			if (rank != HASH_KEY_GET_RANK(hash_obj->hash_table[i].key))
				continue;

			result_total += TO_TIME(hash_obj->hash_table[i].t_tot);
		}
	}

	return result_total;
}

/**
 * ibprof_hash_dump
 *
 * @brief
 *    Dump collected calls.
 *
 * @return formatted string
 ***************************************************************************/
const char *ibprof_hash_dump(IBPROF_HASH_OBJECT *hash_obj,
				int module,
				int call,
				int rank,
				const char *(*format)(int module, const char* call_name, const char* stats_fmt, ...)) {
	static char buffer[1024];
	char *dest = buffer;
	char *call_name = NULL;
	int dest_len = 0;
	int ret = 0;
	int i = 0;

	if (!hash_obj || !format)
		return NULL;

	buffer[0] = '\0';
	for (i = 0; i < hash_obj->size; i++) {
		if (hash_obj->hash_table[i].key != HASH_KEY_INVALID) {
			if (module != HASH_KEY_GET_MODULE(hash_obj->hash_table[i].key))
				continue;

			if (call == UNDEFINED_VALUE) {
				call_name = hash_obj->hash_table[i].call_name;
			} else if (call	!= HASH_KEY_GET_CALL(hash_obj->hash_table[i].key))
				continue;

			if (rank != HASH_KEY_GET_RANK(hash_obj->hash_table[i].key))
				continue;

			switch (ibprof_conf_get_mode(module)) {
			case IBPROF_MODE_ERR:
				ret = sys_snprintf_safe((dest + dest_len),
							(sizeof(buffer) - dest_len), "%s",
							format(module, call_name, "%ld %f %f %f %f %ld",
							hash_obj->hash_table[i].count,
	                        TO_TIME(hash_obj->hash_table[i].t_tot),
	                        TO_TIME(hash_obj->hash_table[i].t_tot) / (hash_obj->hash_table[i].count - ibprof_conf_get_int(IBPROF_WARMUP_NUMBER)),
	                        TO_TIME(hash_obj->hash_table[i].t_max),
	                        TO_TIME(hash_obj->hash_table[i].t_min),
							hash_obj->hash_table[i].mode_data.err));
				break;

			default:
				ret = sys_snprintf_safe((dest + dest_len),
							(sizeof(buffer) - dest_len), "%s",
							format(module, call_name, "%ld %f %f %f %f",
							hash_obj->hash_table[i].count,
	                        TO_TIME(hash_obj->hash_table[i].t_tot),
	                        TO_TIME(hash_obj->hash_table[i].t_tot) / (hash_obj->hash_table[i].count - ibprof_conf_get_int(IBPROF_WARMUP_NUMBER)),
	                        TO_TIME(hash_obj->hash_table[i].t_max),
	                        TO_TIME(hash_obj->hash_table[i].t_min)));
				break;
			}
			if (ret >= 0)
				dest_len += ret;

			if (call == UNDEFINED_VALUE)
				ret = sys_snprintf_safe((dest + dest_len),
				(sizeof(buffer) - dest_len),
				"\n");
			if (ret >= 0)
				dest_len += ret;
		}
	}

	return (ret > 0 ? buffer : NULL);
}
