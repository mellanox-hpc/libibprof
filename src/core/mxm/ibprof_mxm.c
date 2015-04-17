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

#if defined(USE_MXM) && (USE_MXM == 1)

#include <mxm/api/mxm_api.h>

#include "ibprof_mxm.h"

#ifndef MXM_VERSION
#define MXM_VERSION(major, minor) (((major)<<MXM_MAJOR_BIT)|((minor)<<MXM_MINOR_BIT))
#endif

#if MXM_API < MXM_VERSION(2,0)
# error Support MXM API v2.0+ only
#endif

/* MXM API */
#define OP_ON_MEMBERS_LIST(OP) \
	OP(mxm_init) \
	OP(mxm_cleanup) \
	OP(mxm_set_am_handler) \
	OP(mxm_mem_map) \
	OP(mxm_mem_unmap) \
	OP(mxm_mem_get_key) \
	OP(mxm_ep_create) \
	OP(mxm_progress) \
	OP(mxm_wait) \
	OP(mxm_ep_destroy) \
	OP(mxm_ep_get_address) \
	OP(mxm_ep_connect) \
	OP(mxm_ep_disconnect) \
	OP(mxm_conn_ctx_set) \
	OP(mxm_conn_ctx_get) \
	OP(mxm_mq_create) \
	OP(mxm_mq_destroy) \
	OP(mxm_req_send) \
	OP(mxm_req_recv) \
	OP(mxm_req_mprobe) \
	OP(mxm_req_probe) \
	OP(mxm_message_recv) \
	OP(mxm_message_release) \
	OP(mxm_req_cancel_send) \
	OP(mxm_req_cancel_recv) \
	OP(mxm_progress_register) \
	OP(mxm_progress_unregister) \
	OP(mxm_config_read_context_opts) \
	OP(mxm_config_free_context_opts) \
	OP(mxm_config_read_ep_opts) \
	OP(mxm_config_free_ep_opts)

/* Declare MXM API functions to substitute original from (lib)mxm library */
OP_ON_MEMBERS_LIST(DECLARE_TYPE)

/* A structure to store our internal calls depending on a mode */
struct mxm_module_api_t {
	OP_ON_MEMBERS_LIST(DECLARE_STRUCT_MEMBER)
};

#define DECLARE_OPTION_STRUCT(TYPE) \
struct mxm_module_api_t mxm_##TYPE##_funcs = { \
	OP_ON_MEMBERS_LIST(PREFIX##_##TYPE) \
};

static struct module_context_t {
	struct mxm_module_api_t	noble;	/* real call */
	struct mxm_module_api_t	mean;	/* our call */
} mxm_module_context;


#define DEFAULT_SYMVER     NULL

#define check_dlsym(func)  check_dlsymv(func, DEFAULT_SYMVER)
#define check_dlsymv(_func, _ver)  \
	do {                                                                   \
		mxm_module_context.noble._func = sys_dlsym(#_func, _ver);      \
		if (!mxm_module_context.noble._func)                           \
			status = IBPROF_ERR_UNSUPPORTED;                       \
	} while (0)


#define DECLARE_OPTION_FUNCTIONS_PROTOTYPED(TYPE) \
		mxm_error_t TYPE ## mxm_init(mxm_context_opts_t *opts, mxm_h *context_p) \
        { FUNC_BODY_INT(TYPE, mxm_init, opts, context_p) }; \
		void TYPE ## mxm_cleanup(mxm_h context) \
        { FUNC_BODY_VOID(TYPE, mxm_cleanup, context) }; \
		mxm_error_t TYPE ## mxm_set_am_handler(mxm_h context, mxm_hid_t hid, mxm_am_handler_t cb, unsigned flags) \
        { FUNC_BODY_INT(TYPE, mxm_set_am_handler, context, hid, cb, flags) }; \
		mxm_error_t TYPE ## mxm_mem_map(mxm_h context, void **address_p, size_t *length_p, unsigned flags, mxm_mem_key_t *remote_mkey, size_t offset) \
        { FUNC_BODY_INT(TYPE, mxm_mem_map, context, address_p, length_p, flags, remote_mkey, offset) }; \
		mxm_error_t TYPE ## mxm_mem_unmap(mxm_h context, void *address, size_t length, unsigned flags) \
        { FUNC_BODY_INT(TYPE, mxm_mem_unmap, context, address, length, flags) }; \
		mxm_error_t TYPE ## mxm_mem_get_key(mxm_h context, void *address, mxm_mem_key_t *mkey) \
        { FUNC_BODY_INT(TYPE, mxm_mem_get_key, context, address, mkey) }; \
		mxm_error_t TYPE ## mxm_ep_create(mxm_h context, mxm_ep_opts_t *opts, mxm_ep_h *ep_p) \
        { FUNC_BODY_INT(TYPE, mxm_ep_create, context, opts, ep_p) }; \
		mxm_error_t TYPE ## mxm_progress(mxm_h context) \
        { FUNC_BODY_INT(TYPE, mxm_progress, context) }; \
		void TYPE ## mxm_wait(mxm_wait_t *wait) \
        { FUNC_BODY_VOID(TYPE, mxm_wait, wait) }; \
		void TYPE ## mxm_ep_destroy(mxm_ep_h ep) \
        { FUNC_BODY_VOID(TYPE, mxm_ep_destroy, ep) }; \
		mxm_error_t TYPE ## mxm_ep_get_address(mxm_ep_h ep, void *address, size_t *addrlen_p) \
        { FUNC_BODY_INT(TYPE, mxm_ep_get_address, ep, address, addrlen_p) }; \
		mxm_error_t TYPE ## mxm_ep_connect(mxm_ep_h ep, void *address, mxm_conn_h *conn_p) \
        { FUNC_BODY_INT(TYPE, mxm_ep_connect, ep, address, conn_p) }; \
		mxm_error_t TYPE ## mxm_ep_disconnect(mxm_conn_h conn) \
        { FUNC_BODY_INT(TYPE, mxm_ep_disconnect, conn) }; \
		void TYPE ## mxm_conn_ctx_set(mxm_conn_h conn, void *ctx) \
        { FUNC_BODY_VOID(TYPE, mxm_conn_ctx_set, conn, ctx) }; \
		void* TYPE ## mxm_conn_ctx_get(mxm_conn_h conn) \
        { FUNC_BODY_PTR(TYPE, mxm_conn_ctx_get, conn) }; \
		mxm_error_t TYPE ## mxm_mq_create(mxm_h context, mxm_ctxid_t ctxid, mxm_mq_h *mqp) \
        { FUNC_BODY_INT(TYPE, mxm_mq_create, context, ctxid, mqp) }; \
		void TYPE ## mxm_mq_destroy(mxm_mq_h mq) \
        { FUNC_BODY_VOID(TYPE, mxm_mq_destroy, mq) }; \
		mxm_error_t TYPE ## mxm_req_send(mxm_send_req_t *req) \
        { FUNC_BODY_INT(TYPE, mxm_req_send, req) }; \
		mxm_error_t TYPE ## mxm_req_recv(mxm_recv_req_t *req) \
        { FUNC_BODY_INT(TYPE, mxm_req_recv, req) }; \
		mxm_error_t TYPE ## mxm_req_mprobe(mxm_recv_req_t *req, mxm_message_h *msgp) \
        { FUNC_BODY_INT(TYPE, mxm_req_mprobe, req, msgp) }; \
		mxm_error_t TYPE ## mxm_req_probe(mxm_recv_req_t *req) \
        { FUNC_BODY_INT(TYPE, mxm_req_probe, req) }; \
		mxm_error_t TYPE ## mxm_message_recv(mxm_recv_req_t *req, mxm_message_h msg) \
        { FUNC_BODY_INT(TYPE, mxm_message_recv, req, msg) }; \
		mxm_error_t TYPE ## mxm_message_release(mxm_message_h msg) \
        { FUNC_BODY_INT(TYPE, mxm_message_release, msg) }; \
		mxm_error_t TYPE ## mxm_req_cancel_send(mxm_send_req_t *req) \
        { FUNC_BODY_INT(TYPE, mxm_req_cancel_send, req) }; \
		mxm_error_t TYPE ## mxm_req_cancel_recv(mxm_recv_req_t *req) \
        { FUNC_BODY_INT(TYPE, mxm_req_cancel_recv, req) }; \
		mxm_error_t TYPE ## mxm_progress_register(mxm_h context, mxm_progress_cb_t progress_cb, void *arg) \
        { FUNC_BODY_INT(TYPE, mxm_progress_register, context, progress_cb, arg) }; \
		mxm_error_t TYPE ## mxm_progress_unregister(mxm_h context, mxm_progress_cb_t progress_cb) \
        { FUNC_BODY_INT(TYPE, mxm_progress_unregister, context, progress_cb) }; \
		mxm_error_t TYPE ## mxm_config_read_context_opts(mxm_context_opts_t **optsp) \
        { FUNC_BODY_INT(TYPE, mxm_config_read_context_opts, optsp) }; \
		void TYPE ## mxm_config_free_context_opts(mxm_context_opts_t *opts) \
        { FUNC_BODY_VOID(TYPE, mxm_config_free_context_opts, opts) }; \
		mxm_error_t TYPE ## mxm_config_read_ep_opts(mxm_ep_opts_t **optsp) \
        { FUNC_BODY_INT(TYPE, mxm_config_read_ep_opts, optsp) }; \
		void TYPE ## mxm_config_free_ep_opts(mxm_ep_opts_t *opts) \
        { FUNC_BODY_VOID(TYPE, mxm_config_free_ep_opts, opts) }; \


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

static const IBPROF_MODULE_CALL mxm_tbl_call[] =
{
	OP_ON_MEMBERS_LIST(TBL_CALL_ENRTY)
	{UNDEFINED_VALUE, NULL, NULL},
};

static IBPROF_ERROR __mxm_init(IBPROF_MODULE_OBJECT *mod_obj)
{
	IBPROF_ERROR status = IBPROF_ERR_NONE;
    struct mxm_module_api_t* src_api = NULL;

	if ((status = sys_dlcheck("libmxm.so")) != IBPROF_ERR_NONE)
		return status;

	/*
	 * Find the original version of functions we override.
	 */
	check_dlsym(mxm_init);
	check_dlsym(mxm_cleanup);
	check_dlsym(mxm_set_am_handler);
	check_dlsym(mxm_mem_map);
	check_dlsym(mxm_mem_unmap);
	check_dlsym(mxm_mem_get_key);
	check_dlsym(mxm_ep_create);
	check_dlsym(mxm_progress);
	check_dlsym(mxm_wait);
	check_dlsym(mxm_ep_destroy);
	check_dlsym(mxm_ep_get_address);
	check_dlsym(mxm_ep_connect);
	check_dlsym(mxm_ep_disconnect);
	check_dlsym(mxm_conn_ctx_set);
	check_dlsym(mxm_conn_ctx_get);
	check_dlsym(mxm_mq_create);
	check_dlsym(mxm_mq_destroy);
	check_dlsym(mxm_req_send);
	check_dlsym(mxm_req_recv);
	check_dlsym(mxm_req_mprobe);
	check_dlsym(mxm_req_probe);
	check_dlsym(mxm_message_recv);
	check_dlsym(mxm_message_release);
	check_dlsym(mxm_req_cancel_send);
	check_dlsym(mxm_req_cancel_recv);
	check_dlsym(mxm_progress_register);
	check_dlsym(mxm_progress_unregister);
	check_dlsym(mxm_config_read_context_opts);
	check_dlsym(mxm_config_free_context_opts);
	check_dlsym(mxm_config_read_ep_opts);
	check_dlsym(mxm_config_free_ep_opts);

	switch (ibprof_conf_get_int(IBPROF_MODE_MXM)) {
	case IBPROF_MODE_NONE:
		src_api = &mxm_NONE_funcs;
		break;

	case IBPROF_MODE_VERBOSE:
		src_api = &mxm_VERBOSE_funcs;
		break;

	case IBPROF_MODE_PROF:
		src_api = &mxm_PROF_funcs;
		break;

	case IBPROF_MODE_ERR:
		src_api = &mxm_ERR_funcs;
		break;

	case IBPROF_MODE_TRACE:
		src_api = &mxm_TRACE_funcs;
		break;

	default:
		src_api = &mxm_NONE_funcs;
	}

	memcpy(&mxm_module_context.mean, src_api, sizeof(*src_api));

	return status;
}

IBPROF_MODULE_OBJECT mxm_module = {
	IBPROF_MODULE_MXM,
	"libmxm",
	"Mellanox Messaging Accelerator (MXM) provides enhancements to parallel communication " \
	"libraries by fully utilizing the underlying networking infrastructure provided by " \
	"Mellanox HCA/switch hardware.",
	mxm_tbl_call,
	__mxm_init,
	NULL,
	(void *)&mxm_module_context

};
#else
IBPROF_MODULE_OBJECT mxm_module = {
	IBPROF_MODULE_INVALID,
	"libmxm",
	NULL,
	NULL,
	NULL,
	NULL,
	NULL
};
#endif /* USE_MXM */
