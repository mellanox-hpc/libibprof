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
 * @file ibprof_hash.h
 *
 * @brief This file is place for hash types
 *         declaration and operations definition.
 *
 **/
#ifndef _IBPROF_HASH_H_
#define _IBPROF_HASH_H_

#define HASH_KEY    uint64_t
#define HASH_KEY_INVALID    (-1)
#define HASH_MAX_SIZE       (16301) /* Prime number (4049, 8093, 16301, 32573) */

#define HASH_KEY_SET(module, call, rank, size)  \
	(                                           \
	(((uint64_t)(module) << 60) & 0xF000000000000000) |   \
	(((uint64_t)(call)   << 52) & 0x0FF0000000000000) |   \
	(((uint64_t)(rank)   << 36) & 0x000FFFF000000000) |   \
	(((uint64_t)(size)   << 0)  & 0x00000000FFFFFFFF)     \
	)

#define HASH_KEY_GET_MODULE(key)           (int)(((key) & 0xF000000000000000) >> 60)  /* 4bits by offset 59 */
#define HASH_KEY_GET_CALL(key)             (int)(((key) & 0x0FF0000000000000) >> 52)  /* 8bits by offset 51 */
#define HASH_KEY_GET_RANK(key)             (int)(((key) & 0x000FFFF000000000) >> 36)  /* 16bits by offset 35 */
#define HASH_KEY_GET_SIZE(key)             (int)(((key) & 0x00000000FFFFFFFF) >> 0)   /* 32bits by offset 0 */

/**
 * @struct _IBPROF_HASH_OBJ
 * @brief It is an object to be stored
 */
typedef struct _IBPROF_HASH_OBJ {
	double t_min; /**< minimum time spent in a call */
	double t_max; /**< maximum time spent in a call */
	double t_tot; /**< total time spent in a call */
	int64_t count; /**< number of calls */
	HASH_KEY key; /**< key */
	double t_start; /**< start timer */
	char call_name[100];
	union {
		int64_t err;
	} mode_data;
} IBPROF_HASH_OBJ;

/**
 * @struct _IBPROF_HASH_OBJECT
 * @brief Basis Hash container
 */
typedef struct _IBPROF_HASH_OBJECT {
	IBPROF_HASH_OBJ *hash_table; /**< hash table */
	IBPROF_HASH_OBJ *last; /**< last accessed */
	int size; /**< maximum number of elements */
	int count; /**< current count of elements */
} IBPROF_HASH_OBJECT;

/**
 * ibprof_hash_create
 *
 * @brief
 *    Allocates memory for new hash object and set initial values.
 *
 * @retval pointer to new hash object - on success
 * @retval NULL - on failure
 ***************************************************************************/
IBPROF_HASH_OBJECT *ibprof_hash_create(void);

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
void ibprof_hash_destroy(IBPROF_HASH_OBJECT *hash_obj);

/**
 * ibprof_hash_find
 *
 * @brief
 *    Find element in hash object by a key or set one inside.
 *
 * @param[in]    HASH_KEY        key.
 *
 * @retval pointer to hash object - on success
 * @retval NULL - on failure
 ***************************************************************************/
static INLINE IBPROF_HASH_OBJ *ibprof_hash_find(IBPROF_HASH_OBJECT *hash_obj,
						HASH_KEY key)
{
	IBPROF_HASH_OBJ *entry = NULL;
	int attempts = 0;
	int idx = 0;

	if (hash_obj->last && hash_obj->last->key == key)
		return hash_obj->last;

	idx = key % hash_obj->size;

	do {
		entry = &(hash_obj->hash_table[idx]);

		if ((hash_obj->count < hash_obj->size) &&
			(entry->key == HASH_KEY_INVALID)) {
			sys_memset(entry, 0, sizeof(*entry));
			entry->key = key;
			entry->count = 0;
			entry->t_start = UNDEFINED_VALUE;
			entry->t_tot = 0.0;
			entry->t_max = 0.0;
			entry->t_min = DBL_MAX;
			entry->call_name[0]=0;
			hash_obj->count++;
			break;
		} else {
			if (attempts >= (hash_obj->size - 1)) {
				entry = NULL;
				break;
			}
			attempts++;
			idx = (idx + 1) % hash_obj->size;
		}
	} while (entry->key != key);

	hash_obj->last = (entry ? entry : hash_obj->last);

	return entry;
}

/**
 * ibprof_hash_update
 *
 * @brief
 *    Update element in hash object.
 *
 * @return @a none
 ***************************************************************************/
static INLINE void ibprof_hash_update(IBPROF_HASH_OBJECT *hash_obj,
					IBPROF_HASH_OBJ *entry,
					double tm)
{
	if (entry) {
		entry->count++;
		if (entry->count > ibprof_conf_get_int(IBPROF_WARMUP_NUMBER)){
			entry->t_tot += tm;
			entry->t_max = sys_max(entry->t_max, tm);
			entry->t_min = sys_min(entry->t_min, tm);
		}
	}

	return;
}

/**
 * ibprof_hash_update_ex
 *
 * @brief
 *    Update element in hash object.
 *    Provide additional mode related data
 *
 * @return @a none
 ***************************************************************************/
static INLINE void ibprof_hash_update_ex(IBPROF_HASH_OBJECT *hash_obj,
					IBPROF_HASH_OBJ *entry,
					double tm,
					void *ctx)
{
	if (entry) {
		entry->count++;
		if (entry->count > ibprof_conf_get_int(IBPROF_WARMUP_NUMBER)){
			entry->t_tot += tm;
			entry->t_max = sys_max(entry->t_max, tm);
			entry->t_min = sys_min(entry->t_min, tm);
			if (ctx) {
				/* Update mode specific data */
				switch (ibprof_conf_get_mode(HASH_KEY_GET_MODULE(entry->key))) {
				case IBPROF_MODE_ERR:
					entry->mode_data.err += *(int64_t *)ctx;
					break;

				default:
					/* Do nothing */
					break;
				}
			}
		}
	}

	return;
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
		int rank);

/**
 * ibprof_hash_dump
 *
 * @brief
 *    Dump collected calls.
 *
 * @return formatted string
 ***************************************************************************/
const char *ibprof_hash_dump(IBPROF_HASH_OBJECT *hash_obj,
		int module, int call, int rank,
		const char *(*format)(int module, const char* call_name, const char* stats_fmt, ...));

/**
 * ibprof_hash_count
 *
 * @brief
 *    Get number of collected elements.
 *
 * @retval (count) - on success
 ***************************************************************************/
static INLINE int ibprof_hash_count(IBPROF_HASH_OBJECT *hash_obj)
{
	return hash_obj->count;
}

/**
 * ibprof_hash_module_is_empty
 * @brief
 *    Check whether the ibprof module is empty
 * @retval true or false
 ***************************************************************************/
static INLINE int ibprof_hash_module_is_empty(int moduleid, IBPROF_HASH_OBJECT *hash_obj)
{
	int i=0;
	for (i = 0; i < hash_obj->size; i++) {
		if (moduleid == HASH_KEY_GET_MODULE(hash_obj->hash_table[i].key)){
			return 0;
		}
	}
	return 1;
}


#endif /* _IBPROF_HASH_H_ */
