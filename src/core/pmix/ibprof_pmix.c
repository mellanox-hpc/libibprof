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

#if defined(USE_PMIX) && (USE_PMIX == 1)

#include <stdbool.h>
#include <pmix.h>

#include "ibprof_pmix.h"


/* PMIX API */
#define OP_ON_MEMBERS_LIST(OP) \
	OP(PMIx_Init) \
	OP(PMIx_Finalize) \
	OP(PMIx_Initialized) \
	OP(PMIx_Abort) \
	OP(PMIx_Put) \
	OP(PMIx_Commit) \
	OP(PMIx_Fence) \
	OP(PMIx_Fence_nb) \
	OP(PMIx_Get) \
	OP(PMIx_Get_nb) \
	OP(PMIx_Publish) \
	OP(PMIx_Publish_nb) \
	OP(PMIx_Lookup) \
	OP(PMIx_Lookup_nb) \
	OP(PMIx_Unpublish) \
	OP(PMIx_Unpublish_nb) \
	OP(PMIx_Spawn) \
	OP(PMIx_Spawn_nb) \
	OP(PMIx_Connect) \
	OP(PMIx_Connect_nb) \
	OP(PMIx_Disconnect) \
	OP(PMIx_Disconnect_nb) \
	OP(PMIx_Resolve_peers) \
	OP(PMIx_Resolve_nodes)

/* Declare PMIX API functions to substitute original from (lib)pmix library */
OP_ON_MEMBERS_LIST(DECLARE_TYPE)

/* A structure to store our internal calls depending on a mode */
struct pmix_module_api_t {
	OP_ON_MEMBERS_LIST(DECLARE_STRUCT_MEMBER)
};

#define DECLARE_OPTION_STRUCT(TYPE) \
struct pmix_module_api_t pmix_##TYPE##_funcs = { \
	OP_ON_MEMBERS_LIST(PREFIX##_##TYPE) \
};

static struct module_context_t {
	struct pmix_module_api_t	noble;	/* real call */
	struct pmix_module_api_t	mean;	/* our call */
} pmix_module_context;


#define DEFAULT_SYMVER     NULL

#define check_dlsym(func)  check_dlsymv(func, DEFAULT_SYMVER)
#define check_dlsymv(_func, _ver)  \
	do {                                                                   \
		pmix_module_context.noble._func = sys_dlsym(#_func, _ver);     \
		if (!pmix_module_context.noble._func)                          \
			status = IBPROF_ERR_UNSUPPORTED;                       \
	} while (0)


#define DECLARE_OPTION_FUNCTIONS_PROTOTYPED(TYPE) \
		pmix_status_t TYPE ## PMIx_Init(pmix_proc_t *proc) \
        { FUNC_BODY_INT(TYPE, PMIx_Init, proc) }; \
        	pmix_status_t TYPE ## PMIx_Finalize(void) \
        { FUNC_BODY_INT(TYPE, PMIx_Finalize) }; \
		int TYPE ## PMIx_Initialized(void) \
        { FUNC_BODY_INT(TYPE, PMIx_Initialized) }; \
        	pmix_status_t TYPE ## PMIx_Abort(int status, const char msg[], pmix_proc_t procs[], size_t nprocs) \
        { FUNC_BODY_INT(TYPE, PMIx_Abort, status, msg, procs, nprocs) }; \
        	pmix_status_t TYPE ## PMIx_Put(pmix_scope_t scope, const char key[], pmix_value_t *val) \
        { FUNC_BODY_INT(TYPE, PMIx_Put, scope, key, val) }; \
        	pmix_status_t TYPE ## PMIx_Commit(void) \
        { FUNC_BODY_INT(TYPE, PMIx_Commit) }; \
        	pmix_status_t TYPE ## PMIx_Fence(const pmix_proc_t procs[], size_t nprocs, const pmix_info_t info[], size_t ninfo) \
        { FUNC_BODY_INT(TYPE, PMIx_Fence, procs, nprocs, info, ninfo) }; \
        	pmix_status_t TYPE ## PMIx_Fence_nb(const pmix_proc_t procs[], size_t nprocs, const pmix_info_t info[], size_t ninfo, pmix_op_cbfunc_t cbfunc, void *cbdata) \
	{ FUNC_BODY_INT(TYPE, PMIx_Fence_nb, procs, nprocs, info, ninfo, cbfunc, cbdata) }; \
		pmix_status_t TYPE ## PMIx_Get(const pmix_proc_t *proc, const char key[], const pmix_info_t info[], size_t ninfo, pmix_value_t **val) \
	{ FUNC_BODY_INT(TYPE, PMIx_Get, proc, key, info, ninfo, val) }; \
		pmix_status_t TYPE ## PMIx_Get_nb(const pmix_proc_t *proc, const char key[], const pmix_info_t info[], size_t ninfo, pmix_value_cbfunc_t cbfunc, void *cbdata) \
	{ FUNC_BODY_INT(TYPE, PMIx_Get_nb, proc, key, info, ninfo, cbfunc, cbdata) }; \
		pmix_status_t TYPE ## PMIx_Publish(const pmix_info_t info[], size_t ninfo) \
	{ FUNC_BODY_INT(TYPE, PMIx_Publish, info, ninfo) }; \
		pmix_status_t TYPE ## PMIx_Publish_nb(const pmix_info_t info[], size_t ninfo, pmix_op_cbfunc_t cbfunc, void *cbdata) \
	{ FUNC_BODY_INT(TYPE, PMIx_Publish_nb, info, ninfo, cbfunc, cbdata) }; \
		pmix_status_t TYPE ## PMIx_Lookup(pmix_pdata_t data[], size_t ndata, const pmix_info_t info[], size_t ninfo) \
	{ FUNC_BODY_INT(TYPE, PMIx_Lookup, data, ndata, info, ninfo) }; \
		pmix_status_t TYPE ## PMIx_Lookup_nb(char **keys, const pmix_info_t info[], size_t ninfo, pmix_lookup_cbfunc_t cbfunc, void *cbdata) \
	{ FUNC_BODY_INT(TYPE, PMIx_Lookup_nb, keys, info, ninfo, cbfunc, cbdata) }; \
		pmix_status_t TYPE ## PMIx_Unpublish(char **keys, const pmix_info_t info[], size_t ninfo) \
	{ FUNC_BODY_INT(TYPE, PMIx_Unpublish, keys, info, ninfo) }; \
		pmix_status_t TYPE ## PMIx_Unpublish_nb(char **keys, const pmix_info_t info[], size_t ninfo, pmix_op_cbfunc_t cbfunc, void *cbdata) \
	{ FUNC_BODY_INT(TYPE, PMIx_Unpublish_nb, keys, info, ninfo, cbfunc, cbdata) }; \
		pmix_status_t TYPE ## PMIx_Spawn(const pmix_info_t job_info[], size_t ninfo, const pmix_app_t apps[], size_t napps, char nspace[]) \
	{ FUNC_BODY_INT(TYPE, PMIx_Spawn, job_info, ninfo, apps, napps, nspace) }; \
		pmix_status_t TYPE ## PMIx_Spawn_nb(const pmix_info_t job_info[], size_t ninfo, const pmix_app_t apps[], size_t napps, pmix_spawn_cbfunc_t cbfunc, void *cbdata) \
	{ FUNC_BODY_INT(TYPE, PMIx_Spawn_nb, job_info, ninfo, apps, napps, cbfunc, cbdata) }; \
		pmix_status_t TYPE ## PMIx_Connect(const pmix_proc_t procs[], size_t nprocs, const pmix_info_t info[], size_t ninfo) \
	{ FUNC_BODY_INT(TYPE, PMIx_Connect, procs, nprocs, info, ninfo) }; \
		pmix_status_t TYPE ## PMIx_Connect_nb(const pmix_proc_t procs[], size_t nprocs, const pmix_info_t info[], size_t ninfo, pmix_op_cbfunc_t cbfunc, void *cbdata) \
	{ FUNC_BODY_INT(TYPE, PMIx_Connect_nb, procs, nprocs, info, ninfo, cbfunc, cbdata) }; \
		pmix_status_t TYPE ## PMIx_Disconnect(const pmix_proc_t procs[], size_t nprocs, const pmix_info_t info[], size_t ninfo) \
	{ FUNC_BODY_INT(TYPE, PMIx_Disconnect, procs, nprocs, info, ninfo) }; \
		pmix_status_t TYPE ## PMIx_Disconnect_nb(const pmix_proc_t ranges[], size_t nprocs, const pmix_info_t info[], size_t ninfo, pmix_op_cbfunc_t cbfunc, void *cbdata) \
	{ FUNC_BODY_INT(TYPE, PMIx_Disconnect_nb, ranges, nprocs, info, ninfo, cbfunc, cbdata) }; \
		pmix_status_t TYPE ## PMIx_Resolve_peers(const char *nodename, const char *nspace, pmix_proc_t **procs, size_t *nprocs) \
	{ FUNC_BODY_INT(TYPE, PMIx_Resolve_peers, nodename, nspace, procs, nprocs) }; \
		pmix_status_t TYPE ## PMIx_Resolve_nodes(const char *nspace, char **nodelist) \
	{ FUNC_BODY_INT(TYPE, PMIx_Resolve_nodes, nspace, nodelist) };


/****************************************************************************
 * Module declaration place
 ***************************************************************************/

#define DECLARE_OPTION(type) \
DECLARE_OPTION_FUNCTIONS_PROTOTYPED(type) \
DECLARE_OPTION_STRUCT(type)

#if defined(HAVE_VISIBILITY)
#pragma GCC visibility push(default)
#endif
DECLARE_OPTION_FUNCTIONS_PROTOTYPED( )
#if defined(HAVE_VISIBILITY)
#pragma GCC visibility pop
#endif

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

static const IBPROF_MODULE_CALL pmix_tbl_call[] =
{
	OP_ON_MEMBERS_LIST(TBL_CALL_ENRTY)
	{UNDEFINED_VALUE, NULL, NULL},
};

static IBPROF_ERROR __pmix_init(IBPROF_MODULE_OBJECT *mod_obj)
{
	IBPROF_ERROR status = IBPROF_ERR_NONE;
	struct pmix_module_api_t* src_api = NULL;

	if ((status = sys_dlcheck("libpmix.so")) != IBPROF_ERR_NONE)
		return status;

	/*
	 * Find the original version of functions we override.
	 */
	check_dlsym(PMIx_Init);
	check_dlsym(PMIx_Finalize);
	check_dlsym(PMIx_Initialized);
	check_dlsym(PMIx_Abort);
	check_dlsym(PMIx_Put);
	check_dlsym(PMIx_Commit);
	check_dlsym(PMIx_Fence);
	check_dlsym(PMIx_Fence_nb);
	check_dlsym(PMIx_Get);
	check_dlsym(PMIx_Get_nb);
	check_dlsym(PMIx_Publish);
	check_dlsym(PMIx_Publish_nb);
	check_dlsym(PMIx_Lookup);
	check_dlsym(PMIx_Lookup_nb);
	check_dlsym(PMIx_Unpublish);
	check_dlsym(PMIx_Unpublish_nb);
	check_dlsym(PMIx_Spawn);
	check_dlsym(PMIx_Spawn_nb);
	check_dlsym(PMIx_Connect);
	check_dlsym(PMIx_Connect_nb);
	check_dlsym(PMIx_Disconnect);
	check_dlsym(PMIx_Disconnect_nb);
	check_dlsym(PMIx_Resolve_peers);
	check_dlsym(PMIx_Resolve_nodes);

	switch (ibprof_conf_get_int(IBPROF_MODE_PMIX)) {
	case IBPROF_MODE_NONE:
		src_api = &pmix_NONE_funcs;
		break;

	case IBPROF_MODE_VERBOSE:
		src_api = &pmix_VERBOSE_funcs;
		break;

	case IBPROF_MODE_PROF:
		src_api = &pmix_PROF_funcs;
		break;

	case IBPROF_MODE_ERR:
		src_api = &pmix_ERR_funcs;
		break;

	case IBPROF_MODE_TRACE:
		src_api = &pmix_TRACE_funcs;
		break;

	default:
		src_api = &pmix_NONE_funcs;
	}

	memcpy(&pmix_module_context.mean, src_api, sizeof(*src_api));

	return status;
}

IBPROF_MODULE_OBJECT pmix_module = {
	IBPROF_MODULE_PMIX,
	"libpmix",
	"PMI Exascale (PMIx) represents an attempt to resolve scaling properties by providing an " \
	" extended version of the PMI standard specifically designed to support clusters up to and " \
	" including exascale sizes",
	pmix_tbl_call,
	__pmix_init,
	NULL,
	(void *)&pmix_module_context
};
#else
IBPROF_MODULE_OBJECT pmix_module = {
	IBPROF_MODULE_INVALID,
	"libpmix",
	NULL,
	NULL,
	NULL,
	NULL,
	NULL
};
#endif /* USE_PMIX */
