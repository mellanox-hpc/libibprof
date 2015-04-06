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

#if defined(USE_HCOL) && (USE_HCOL == 1)

#include <hcoll/api/hcoll_dte.h>

#include "ibprof_hcol.h"

int hmca_coll_ml_barrier_intra(void *context);
int hmca_coll_ml_bcast_sequential_root(void *buf, int count, dte_data_representation_t dtype, int root, void* hcoll_context);
int hmca_coll_ml_parallel_bcast(void *buf, int count, dte_data_representation_t dtype, int root, void *hcoll_context);
int hmca_coll_ml_alltoall(void *sbuf, int scount, dte_data_representation_t sdtype, void* rbuf, int rcount, dte_data_representation_t rdtype, void *hcoll_context);
int hmca_coll_ml_allreduce_dispatch(void *sbuf, void *rbuf, int count, dte_data_representation_t dtype, hcoll_dte_op_t *op, void *hcoll_context);
int hmca_coll_ml_allreduce(void *sbuf, void *rbuf, int count, dte_data_representation_t dtype, hcoll_dte_op_t *op, void *hcoll_context);
int hmca_coll_ml_allgather(void *sbuf, int scount, dte_data_representation_t sdtype, void* rbuf, int rcount, dte_data_representation_t rdtype, void *hcoll_context);


/* HCOL API */
#define OP_ON_MEMBERS_LIST(OP) \
	OP(hmca_coll_ml_barrier_intra) \
	OP(hmca_coll_ml_bcast_sequential_root) \
	OP(hmca_coll_ml_parallel_bcast) \
	OP(hmca_coll_ml_alltoall) \
	OP(hmca_coll_ml_allreduce_dispatch) \
	OP(hmca_coll_ml_allreduce) \
	OP(hmca_coll_ml_allgather)

/* Declare HCOL API functions to substitute original from (lib)hcol library */
OP_ON_MEMBERS_LIST(DECLARE_TYPE)

/* A structure to store our internal calls depending on a mode */
struct hcol_module_api_t {
	OP_ON_MEMBERS_LIST(DECLARE_STRUCT_MEMBER)
};

#define DECLARE_OPTION_STRUCT(TYPE) \
struct hcol_module_api_t hcol_##TYPE##_funcs = { \
	OP_ON_MEMBERS_LIST(PREFIX##_##TYPE) \
};

static struct module_context_t {
	struct hcol_module_api_t	noble;	/* real call */
	struct hcol_module_api_t	mean;	/* our call */
} hcol_module_context;


#define DEFAULT_SYMVER     NULL

#define check_dlsym(func)  check_dlsymv(func, DEFAULT_SYMVER)
#define check_dlsymv(_func, _ver)  \
	do {                                                                   \
		hcol_module_context.noble._func = sys_dlsym(#_func, _ver);     \
		if (!hcol_module_context.noble._func)                          \
			status = IBPROF_ERR_UNSUPPORTED;                       \
	} while (0)


#define DECLARE_OPTION_FUNCTIONS_PROTOTYPED(TYPE) \
		int TYPE ## hmca_coll_ml_barrier_intra(void *context) \
        { FUNC_BODY_INT(TYPE, hmca_coll_ml_barrier_intra, context) }; \
		int TYPE ## hmca_coll_ml_bcast_sequential_root(void *buf, int count, dte_data_representation_t dtype, int root, void* hcoll_context) \
        { FUNC_BODY_INT(TYPE, hmca_coll_ml_bcast_sequential_root, buf, count, dtype, root, hcoll_context) }; \
		int TYPE ## hmca_coll_ml_parallel_bcast(void *buf, int count, dte_data_representation_t dtype, int root, void *hcoll_context) \
        { FUNC_BODY_INT(TYPE, hmca_coll_ml_parallel_bcast, buf, count, dtype, root, hcoll_context) }; \
		int TYPE ## hmca_coll_ml_alltoall(void *sbuf, int scount, dte_data_representation_t sdtype, void* rbuf, int rcount, dte_data_representation_t rdtype, void *hcoll_context) \
        { FUNC_BODY_INT(TYPE, hmca_coll_ml_alltoall, sbuf, scount, sdtype, rbuf, rcount, rdtype, hcoll_context) }; \
		int TYPE ## hmca_coll_ml_allreduce_dispatch(void *sbuf, void *rbuf, int count, dte_data_representation_t dtype, hcoll_dte_op_t *op, void *hcoll_context) \
        { FUNC_BODY_INT(TYPE, hmca_coll_ml_allreduce_dispatch, sbuf,rbuf, count, dtype, op, hcoll_context) }; \
		int TYPE ## hmca_coll_ml_allreduce(void *sbuf, void *rbuf, int count, dte_data_representation_t dtype, hcoll_dte_op_t *op, void *hcoll_context) \
        { FUNC_BODY_INT(TYPE, hmca_coll_ml_allreduce, sbuf, rbuf, count, dtype, op, hcoll_context) }; \
		int TYPE ## hmca_coll_ml_allgather(void *sbuf, int scount, dte_data_representation_t sdtype, void* rbuf, int rcount, dte_data_representation_t rdtype, void *hcoll_context) \
        { FUNC_BODY_INT(TYPE, hmca_coll_ml_allgather, sbuf, scount, sdtype, rbuf, rcount, rdtype, hcoll_context) }; \


/****************************************************************************
 * Module declaration place
 ***************************************************************************/

#define DECLARE_OPTION(type) \
DECLARE_OPTION_FUNCTIONS_PROTOTYPED(type) \
DECLARE_OPTION_STRUCT(type)

DECLARE_OPTION_FUNCTIONS_PROTOTYPED( )

#define PREFIX_NONE(x) NONE##x,
DECLARE_OPTION(NONE)

#define PREFIX_PROF(x) PROF##x,
DECLARE_OPTION(PROF)

#define PREFIX_VERBOSE(x) VERBOSE##x,
DECLARE_OPTION(VERBOSE)

#define PREFIX_TRACE(x) TRACE##x,
DECLARE_OPTION(TRACE)

#define PREFIX_ERR(x) ERR##x,
DECLARE_OPTION(ERR)

/****************************************************************************
 * Module configuration place
 ***************************************************************************/

static const IBPROF_MODULE_CALL hcol_tbl_call[] =
{
	OP_ON_MEMBERS_LIST(TBL_CALL_ENRTY)
	{UNDEFINED_VALUE, NULL, NULL},
};

static IBPROF_ERROR __hcol_init(IBPROF_MODULE_OBJECT *mod_obj)
{
	IBPROF_ERROR status = IBPROF_ERR_NONE;
    struct hcol_module_api_t* src_api = NULL;

	if ((status = sys_dlcheck("libhcoll.so")) != IBPROF_ERR_NONE)
		return status;

	/*
	 * Find the original version of functions we override.
	 */
	check_dlsym(hmca_coll_ml_barrier_intra);
	check_dlsym(hmca_coll_ml_bcast_sequential_root);
	check_dlsym(hmca_coll_ml_parallel_bcast);
	check_dlsym(hmca_coll_ml_alltoall);
	check_dlsym(hmca_coll_ml_allreduce_dispatch);
	check_dlsym(hmca_coll_ml_allreduce);
	check_dlsym(hmca_coll_ml_allgather);


	switch (ibprof_conf_get_int(IBPROF_MODE_HCOL)) {
	case IBPROF_MODE_NONE:
		src_api = &hcol_NONE_funcs;
		break;

	case IBPROF_MODE_VERBOSE:
		src_api = &hcol_VERBOSE_funcs;
		break;

	case IBPROF_MODE_PROF:
		src_api = &hcol_PROF_funcs;
		break;

	case IBPROF_MODE_ERR:
		src_api = &hcol_ERR_funcs;
		break;

	case IBPROF_MODE_TRACE:
		src_api = &hcol_TRACE_funcs;
		break;

	default:
		src_api = &hcol_NONE_funcs;
	}

	memcpy(&hcol_module_context.mean, src_api, sizeof(*src_api));

	return status;
}

IBPROF_MODULE_OBJECT hcol_module = {
	IBPROF_MODULE_HCOL,
	"libhcol",
	"hcol is a library having collective operations",
	hcol_tbl_call,
	__hcol_init,
	NULL,
	(void *)&hcol_module_context
};
#else
IBPROF_MODULE_OBJECT hcol_module = {
	IBPROF_MODULE_INVALID,
	"libhcol",
	NULL,
	NULL,
	NULL,
	NULL,
	NULL
};
#endif /* USE_HCOL */
