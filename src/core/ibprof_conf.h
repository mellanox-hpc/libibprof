/*
 * Copyright (c) 2013-2015 Mellanox Technologies, Inc.
 *                         All rights reserved.
 * $COPYRIGHT$
 *
 * Additional copyrights may follow
 *
 * $HEADER$
 */

#ifndef _IBPROF_ENV_H_
#define _IBPROF_ENV_H_

typedef enum {
	IBPROF_TEST_MASK = 0X0,
	IBPROF_MODE_IBV,
	IBPROF_MODE_HCOL,
	IBPROF_MODE_MXM,
	IBPROF_MODE_PMIX,
	IBPROF_DUMP_FILE,
	IBPROF_WARMUP_NUMBER,
	IBPROF_OUTPUT_PREFIX,
	IBPROF_FORMAT,
	IBPROF_ERR_PERCENT,
	IBPROF_ERR_SEED,

	IBPROF_ENV_OPTIONS_AMOUNT
} IBPROF_ENV;

void ibprof_conf_init(void);

char* ibprof_conf_get_string(IBPROF_ENV variable);;

int ibprof_conf_get_int(IBPROF_ENV variable);

int ibprof_conf_get_mode(int module);

#endif /* _IBPROF_ENV_H_ */
