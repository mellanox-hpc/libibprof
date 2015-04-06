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
 * @file ibprof_types.h
 *
 * @brief Common types, structures etc for library.
 *
 **/
#ifndef _IBPROF_TYPES_H_
#define _IBPROF_TYPES_H_

#include "ibprof_cmn.h"
#include "ibprof_api.h"
#include "ibprof_task.h"
#include "ibprof_hash.h"

#define ibprof_timestamp_diff(t_val)   (ibprof_timestamp() - (t_val))


/**
 * @struct _IBPROF_OBJECT
 * @brief Basis object
 */
typedef struct _IBPROF_OBJECT {
	IBPROF_MODULE_OBJECT **module_array; /**< array of available modules */
	IBPROF_HASH_OBJECT *hash_obj; /**< hash object */
	IBPROF_TASK_OBJECT *task_obj; /**< task object */
	CRITICAL_SECTION lock; /**< protection object */
} IBPROF_OBJECT;


#endif /* _IBPROF_TYPES_H_ */
