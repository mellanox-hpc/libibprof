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

#include "ibprof_task.h"

/****************************************************************************
 * Static Function Declarations
 ***************************************************************************/
static void __task_get_cmd(char *, char *);

/**
 * ibprof_task_create
 *
 * @brief
 *    Allocates memory for new task object and set initial values.
 *
 * @retval pointer to new task object - on success
 * @retval NULL - on failure
 ***************************************************************************/
IBPROF_TASK_OBJECT *ibprof_task_create(void)
{
	IBPROF_TASK_OBJECT *task_obj = NULL;

	task_obj = (IBPROF_TASK_OBJECT *) sys_malloc(sizeof(IBPROF_TASK_OBJECT));
	if (task_obj) {
		sys_time(&task_obj->t_start);
		task_obj->date = sys_hostdate();
		task_obj->host = sys_host();
		task_obj->user = sys_user();
		task_obj->jobid = sys_jobid();
		task_obj->procid = sys_procid();
		task_obj->pid = getpid();
		task_obj->tid = sys_threadid();
		task_obj->wall_time = .0;
		__task_get_cmd(task_obj->cmdline, task_obj->cmdpath);
	}

	return task_obj;
}

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
void ibprof_task_destroy(IBPROF_TASK_OBJECT *task_obj)
{
	if (task_obj) {
		sys_free(task_obj->date);
		sys_free(task_obj->host);
		sys_free(task_obj->user);

		sys_free(task_obj);
	}
}


static void __task_get_cmd(char *cmd, char *path)
{
	int i, j;
	FILE *fh;

	cmd[0] = '\0';
	fh = sys_fopen("/proc/self/cmdline", "r");
	if (fh) {
		j = 0;
		if (sys_fgets(cmd, TAST_MAX_STR, fh) != NULL) {
            for (i = 1; i < TAST_MAX_STR; i++) {
                if (cmd[i] == 0 && j == 0) {
                    cmd[i] = ' ';
                    j = 1;
                } else if (cmd[i] == 0 && j == 1) {
                    break;
                } else {
                    j = 0;
                }
            }
        }
		sys_fclose(fh);
	} else {
		sys_sprintf(cmd, "unknown");
	}

	if ((readlink("/proc/self/exe", path, TAST_MAX_STR)) < 1)
		sys_sprintf(path, "unknown");
}
