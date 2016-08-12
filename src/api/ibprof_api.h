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
 * @file ibprof_api.h
 *
 * @brief Library API definition.
 *
 **/
#ifndef _IBPROF_API_H_
#define _IBPROF_API_H_

#ifdef __cplusplus
extern "C" {
#endif


/**
 * @enum
 * @brief List of supported module types.
 */
enum {
	IBPROF_MODULE_IBV = 0,       /**< libibverbs */
	IBPROF_MODULE_HCOL,          /**< libhcoll */
	IBPROF_MODULE_MXM,           /**< libmxm */
	IBPROF_MODULE_PMIX,          /**< libpmix */
	IBPROF_MODULE_SHMEM,        /**< libshmem */
	IBPROF_MODULE_USER,          /**< user defined */
	IBPROF_MODULE_INVALID        /**< invalid module */
};


/**
 * ibprof_timestamp
 *
 * @brief
 *    This function returns current time in seconds
 *
 * @retval (value) - time value in seconds
 ***************************************************************************/
double ibprof_timestamp(void);

/**
 * ibprof_update
 *
 * @brief
 *    Store time duration per module-call pair.
 *
 * @param[in]    module         Module this measure is for.
 * @param[in]    call           Call/function this measure is for.
 * @param[in]    tm             Time value.
 *
 * @retval none
 ***************************************************************************/
void ibprof_update(int module, int call, double tm);

/**
 * ibprof_update_ex
 *
 * @brief
 *    Store time duration per module-call pair.
 *
 * @param[in]    module         Module this measure is for.
 * @param[in]    call           Call/function this measure is for.
 * @param[in]    tm             Time value.
 * @param[in]    ctx            Context related this measurement.
 *
 * @retval none
 ***************************************************************************/
void ibprof_update_ex(int module, int call, double tm, void *ctx);

/**
 * ibprof_interval_start
 *
 * @brief
 *    This function labels starting point of measurement.
 *
 * @param[in]    callid         Unique ID for measurement.
 * @param[in]    name           String name associated with measurement.
 *
 * @retval none
 ***************************************************************************/
void ibprof_interval_start(int callid, const char* name);

/**
 * ibprof_interval_end
 *
 * @brief
 *    This function labels finishing of measurement.
 *
 * @param[in]    callid         Unique ID for measurement.
 *
 * @retval none
 ***************************************************************************/
void ibprof_interval_end(int callid);

/**
 * ibprof_dump
 *
 * @brief
 *    This function dump all collected data in different formats.
 *    Output can be controlled by environment variables.
 *
 * @retval none
 ***************************************************************************/
void ibprof_dump(void);


#ifdef __cplusplus
}
#endif

#endif /* _IBPROF_API_H_ */

