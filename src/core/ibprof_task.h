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
 * @file ibprof_task.h
 *
 * @brief This file is place for task
 *         related data.
 *
 **/
#ifndef _IBPROF_TASK_H_
#define _IBPROF_TASK_H_

#define TAST_MAX_STR   500

/**
 * @struct _IBPROF_TASK_OBJECT
 * @brief Basis Task container
 */
typedef struct _IBPROF_TASK_OBJECT {
	struct timeval t_start;

	char *date;
	char *host;
	char *user;
	int  jobid;
	int  procid;
	int  pid;
	int  tid;
	char cmdline[TAST_MAX_STR];
	char cmdpath[TAST_MAX_STR];
	double wall_time;
} IBPROF_TASK_OBJECT;

/**
 * ibprof_task_create
 *
 * @brief
 *    Allocates memory for new task object and set initial values.
 *
 * @retval pointer to new task object - on success
 * @retval NULL - on failure
 ***************************************************************************/
IBPROF_TASK_OBJECT *ibprof_task_create(void);

/**
 * ibprof_task_destroy
 *
 * @brief
 *    Releases all used resources and free memory allocated for internal object.
 *
 * @param[in]    task_obj        Task object.
 *
 * @return @a none
 ***************************************************************************/
void ibprof_task_destroy(IBPROF_TASK_OBJECT *task_obj);

/**
 * ibprof_task_wall_time
 *
 * @brief
 *    Return a double which represents wall time from input in seconds.
 * @t timeval struct.
 *
 * @retval time in seconds
 ***************************************************************************/
static INLINE double ibprof_task_wall_time(struct timeval t_start)
{
	struct timeval t_end;

	sys_time(&t_end);

	return (t_end.tv_sec - t_start.tv_sec)
		+ (t_end.tv_usec - t_start.tv_usec) * 1.0e-6;
}



#endif /* _IBPROF_HASH_H_ */
