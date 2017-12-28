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

#if defined(USE_IBV) && (USE_IBV == 1)

#include <infiniband/verbs.h>
#include <infiniband/arch.h>

#include "ibprof_ibv.h"

#define DEFAULT_SYMVER     "IBVERBS_1.1"

struct ibv_ctx_t {
	uintptr_t                addr;
#if defined(IBV_API_EXT)
	struct verbs_context     item;
#if (IBV_API_EXT > 1)
	struct verbs_context_exp item_exp;
# endif
#else
	struct ibv_context       item;
#endif /* IBV_API_EXT */
	struct ibv_ctx_t        *next;
};

/* Legacy VERBS API */
/* libibverbs 1.1.2 and earlier differs in legacy API */

#ifdef IBV_API_LEGACY
	#define HAVE_IBV_REG_MR_FUNC(TYPE) \
		struct ibv_mr* TYPE ## ibv_reg_mr(struct ibv_pd *pd, void *addr, size_t length, enum ibv_access_flags access) \
		{ FUNC_BODY_PTR(TYPE, _, ibv_reg_mr, reg_mr, , pd, addr, length, access) }
	#define HAVE_IBV_MODIFY_QP_FUNC(TYPE) \
		int TYPE ## ibv_modify_qp(struct ibv_qp *qp, struct ibv_qp_attr *attr, enum ibv_qp_attr_mask attr_mask) \
		{ FUNC_BODY_INT(TYPE, _, ibv_modify_qp, modify_qp, , qp, attr, attr_mask) }
	#define HAVE_IBV_QUERY_QP_FUNC(TYPE) \
		int TYPE ## ibv_query_qp(struct ibv_qp *qp, struct ibv_qp_attr *attr, enum ibv_qp_attr_mask attr_mask, struct ibv_qp_init_attr *init_attr) \
		{ FUNC_BODY_INT(TYPE, _, ibv_query_qp, query_qp, , qp, attr, attr_mask, init_attr) }
	#define HAVE_IBV_ATTACH_MCAST_FUNC(TYPE) \
		int TYPE ## ibv_attach_mcast(struct ibv_qp *qp, union ibv_gid *gid, uint16_t lid) \
		{ FUNC_BODY_INT(TYPE, _, ibv_attach_mcast, attach_mcast, , qp, gid, lid) }
	#define HAVE_IBV_DETACH_MCAST_FUNC(TYPE) \
		int TYPE ## ibv_detach_mcast(struct ibv_qp *qp, union ibv_gid *gid, uint16_t lid) \
		{ FUNC_BODY_INT(TYPE, _, ibv_detach_mcast, detach_mcast, , qp, gid, lid) }
#else
	#define HAVE_IBV_REG_MR_FUNC(TYPE) \
		struct ibv_mr* TYPE ## ibv_reg_mr(struct ibv_pd *pd, void *addr, size_t length, int access) \
		{ FUNC_BODY_PTR(TYPE, _, ibv_reg_mr, reg_mr, , pd, addr, length, access) }
	#define HAVE_IBV_MODIFY_QP_FUNC(TYPE) \
		int TYPE ## ibv_modify_qp(struct ibv_qp *qp, struct ibv_qp_attr *attr, int attr_mask) \
		{ FUNC_BODY_INT(TYPE, _, ibv_modify_qp, modify_qp, , qp, attr, attr_mask) }
	#define HAVE_IBV_QUERY_QP_FUNC(TYPE) \
		int TYPE ## ibv_query_qp(struct ibv_qp *qp, struct ibv_qp_attr *attr, int attr_mask, struct ibv_qp_init_attr *init_attr) \
		{ FUNC_BODY_INT(TYPE, _, ibv_query_qp, query_qp, , qp, attr, attr_mask, init_attr) }
	#define HAVE_IBV_ATTACH_MCAST_FUNC(TYPE) \
		int TYPE ## ibv_attach_mcast(struct ibv_qp *qp, const union ibv_gid *gid, uint16_t lid) \
		{ FUNC_BODY_INT(TYPE, _, ibv_attach_mcast, attach_mcast, , qp, gid, lid) }
	#define HAVE_IBV_DETACH_MCAST_FUNC(TYPE) \
		int TYPE ## ibv_detach_mcast(struct ibv_qp *qp, const union ibv_gid *gid, uint16_t lid) \
		{ FUNC_BODY_INT(TYPE, _, ibv_detach_mcast, detach_mcast, , qp, gid, lid) }
#endif

/* Extended VERBS API */

#ifdef HAVE_IBV_OPEN_QP
	#define HAVE_IBV_OPEN_QP_FUNC(TYPE) \
        struct ibv_qp* TYPE ## ibv_open_qp(struct ibv_context *context, struct ibv_qp_open_attr *attr) \
        { FUNC_BODY_PTR(TYPE, _EX, ibv_open_qp, open_qp, context, context, attr) }
	#define HAVE_IBV_OPEN_QP_OP(OP) \
		OP(ibv_open_qp)
	#define HAVE_IBV_OPEN_QP_CHECK() \
		check_api_ex(ibv_open_qp, open_qp)
#else
	#define HAVE_IBV_OPEN_QP_FUNC(TYPE)
	#define HAVE_IBV_OPEN_QP_OP(OP)
	#define HAVE_IBV_OPEN_QP_CHECK()
#endif

#ifdef HAVE_IBV_CREATE_QP_EX
	#define HAVE_IBV_CREATE_QP_EX_FUNC(TYPE) \
        struct ibv_qp* TYPE ## ibv_create_qp_ex(struct ibv_context *context, struct ibv_qp_init_attr_ex *attr) \
        { FUNC_BODY_PTR(TYPE, _EX, ibv_create_qp_ex, create_qp_ex, context, context, attr) }
	#define HAVE_IBV_CREATE_QP_EX_OP(OP) \
		OP(ibv_create_qp_ex)
	#define HAVE_IBV_CREATE_QP_EX_CHECK() \
		check_api_ex(ibv_create_qp_ex, create_qp_ex)
#else
	#define HAVE_IBV_CREATE_QP_EX_FUNC(TYPE)
	#define HAVE_IBV_CREATE_QP_EX_OP(OP)
	#define HAVE_IBV_CREATE_QP_EX_CHECK()
#endif

#ifdef HAVE_IBV_OPEN_XRCD
	#define HAVE_IBV_OPEN_XRCD_FUNC(TYPE) \
        struct ibv_xrcd* TYPE ## ibv_open_xrcd(struct ibv_context *context, struct ibv_xrcd_init_attr *xrcd_init_attr) \
        { FUNC_BODY_PTR(TYPE, _EX, ibv_open_xrcd, open_xrcd, context, context, xrcd_init_attr) }
	#define HAVE_IBV_OPEN_XRCD_OP(OP) \
		OP(ibv_open_xrcd)
	#define HAVE_IBV_OPEN_XRCD_CHECK() \
		check_api_ex(ibv_open_xrcd, open_xrcd)
#else
	#define HAVE_IBV_OPEN_XRCD_FUNC(TYPE)
	#define HAVE_IBV_OPEN_XRCD_OP(OP)
	#define HAVE_IBV_OPEN_XRCD_CHECK()
#endif

#ifdef HAVE_IBV_CLOSE_XRCD
	#define HAVE_IBV_CLOSE_XRCD_FUNC(TYPE) \
        int TYPE ## ibv_close_xrcd(struct ibv_xrcd *xrcd) \
        { FUNC_BODY_INT(TYPE, _EX, ibv_close_xrcd, close_xrcd, xrcd->context, xrcd) };
	#define HAVE_IBV_CLOSE_XRCD_OP(OP) \
		OP(ibv_close_xrcd)
	#define HAVE_IBV_CLOSE_XRCD_CHECK() \
		check_api_ex(ibv_close_xrcd, close_xrcd)
#else
	#define HAVE_IBV_CLOSE_XRCD_FUNC(TYPE)
	#define HAVE_IBV_CLOSE_XRCD_OP(OP)
	#define HAVE_IBV_CLOSE_XRCD_CHECK()
#endif


/* Experimental VERBS API */

#ifdef HAVE_IBV_EXP_QUERY_DEVICE
	#define HAVE_IBV_EXP_QUERY_DEVICE_FUNC(TYPE) \
        int TYPE ## ibv_exp_query_device(struct ibv_context *context, struct ibv_exp_device_attr *attr) \
        { FUNC_BODY_INT(TYPE, _EXP, ibv_exp_query_device, lib_exp_query_device, context, context, attr) }
	#define HAVE_IBV_EXP_QUERY_DEVICE_OP(OP) \
		OP(ibv_exp_query_device)
	#define HAVE_IBV_EXP_QUERY_DEVICE_CHECK() \
		check_api_exp(ibv_exp_query_device, lib_exp_query_device)
#else
	#define HAVE_IBV_EXP_QUERY_DEVICE_FUNC(TYPE)
	#define HAVE_IBV_EXP_QUERY_DEVICE_OP(OP)
	#define HAVE_IBV_EXP_QUERY_DEVICE_CHECK()
#endif

#ifdef HAVE_IBV_EXP_MODIFY_CQ
	#define HAVE_IBV_EXP_MODIFY_CQ_FUNC(TYPE) \
        int TYPE ## ibv_exp_modify_cq(struct ibv_cq *cq, struct ibv_exp_cq_attr *attr, int attr_mask) \
        { FUNC_BODY_INT(TYPE, _EXP, ibv_exp_modify_cq, lib_exp_modify_cq, cq->context, cq, attr, attr_mask) }
	#define HAVE_IBV_EXP_MODIFY_CQ_OP(OP) \
		OP(ibv_exp_modify_cq)
	#define HAVE_IBV_EXP_MODIFY_CQ_CHECK() \
		check_api_exp(ibv_exp_modify_cq, lib_exp_modify_cq)
#else
	#define HAVE_IBV_EXP_MODIFY_CQ_FUNC(TYPE)
	#define HAVE_IBV_EXP_MODIFY_CQ_OP(OP)
	#define HAVE_IBV_EXP_MODIFY_CQ_CHECK()
#endif

#ifdef HAVE_IBV_EXP_CREATE_FLOW
	#define HAVE_IBV_EXP_CREATE_FLOW_FUNC(TYPE) \
        struct ibv_exp_flow* TYPE ## ibv_exp_create_flow(struct ibv_qp *qp, struct ibv_exp_flow_attr *flow_attr) \
        { FUNC_BODY_PTR(TYPE, _EXP, ibv_exp_create_flow, lib_exp_ibv_create_flow, qp->context, qp, flow_attr) }
	#define HAVE_IBV_EXP_CREATE_FLOW_OP(OP) \
		OP(ibv_exp_create_flow)
	#define HAVE_IBV_EXP_CREATE_FLOW_CHECK() \
		check_api_exp(ibv_exp_create_flow, lib_exp_ibv_create_flow)
#else
	#define HAVE_IBV_EXP_CREATE_FLOW_FUNC(TYPE)
	#define HAVE_IBV_EXP_CREATE_FLOW_OP(OP)
	#define HAVE_IBV_EXP_CREATE_FLOW_CHECK()
#endif

#ifdef HAVE_IBV_EXP_DESTROY_FLOW
	#define HAVE_IBV_EXP_DESTROY_FLOW_FUNC(TYPE) \
        int TYPE ## ibv_exp_destroy_flow(struct ibv_exp_flow *flow_id) \
        { FUNC_BODY_INT(TYPE, _EXP, ibv_exp_destroy_flow, lib_exp_ibv_destroy_flow, flow_id->context, flow_id) }
	#define HAVE_IBV_EXP_DESTROY_FLOW_OP(OP) \
		OP(ibv_exp_destroy_flow)
	#define HAVE_IBV_EXP_DESTROY_FLOW_CHECK() \
		check_api_exp(ibv_exp_destroy_flow, lib_exp_ibv_destroy_flow)
#else
	#define HAVE_IBV_EXP_DESTROY_FLOW_FUNC(TYPE)
	#define HAVE_IBV_EXP_DESTROY_FLOW_OP(OP)
	#define HAVE_IBV_EXP_DESTROY_FLOW_CHECK()
#endif

#ifdef HAVE_IBV_EXP_POST_TASK
	#define HAVE_IBV_EXP_POST_TASK_FUNC(TYPE) \
        int TYPE ## ibv_exp_post_task(struct ibv_context *context, struct ibv_exp_task *task_list, struct ibv_exp_task **bad_task) \
        { FUNC_BODY_INT(TYPE, _EXP, ibv_exp_post_task, lib_exp_post_task, context, context, task_list, bad_task) }
	#define HAVE_IBV_EXP_POST_TASK_OP(OP) \
		OP(ibv_exp_post_task)
	#define HAVE_IBV_EXP_POST_TASK_CHECK() \
		check_api_exp(ibv_exp_post_task, lib_exp_post_task)
#else
	#define HAVE_IBV_EXP_POST_TASK_FUNC(TYPE)
	#define HAVE_IBV_EXP_POST_TASK_OP(OP)
	#define HAVE_IBV_EXP_POST_TASK_CHECK()
#endif

#ifdef HAVE_IBV_EXP_POLL_CQ_QP
	#define HAVE_IBV_EXP_POLL_CQ_FUNC(TYPE) \
        int TYPE ## ibv_exp_poll_cq(struct ibv_cq *ibcq, int num_entries, struct ibv_exp_wc *wc, uint32_t wc_size) \
        { FUNC_BODY_INT(TYPE, _EXP, ibv_exp_poll_cq, drv_exp_ibv_poll_cq, ibcq->context, ibcq, num_entries, wc, wc_size) }
	#define HAVE_IBV_EXP_POLL_CQ_OP(OP) \
		OP(ibv_exp_poll_cq)
	#define HAVE_IBV_EXP_POLL_CQ_CHECK() \
		check_api_exp(ibv_exp_poll_cq, drv_exp_ibv_poll_cq)
#else
	#define HAVE_IBV_EXP_POLL_CQ_FUNC(TYPE)
	#define HAVE_IBV_EXP_POLL_CQ_OP(OP)
	#define HAVE_IBV_EXP_POLL_CQ_CHECK()
#endif

#ifdef HAVE_IBV_EXP_CREATE_QP
	#define HAVE_IBV_EXP_CREATE_QP_FUNC(TYPE) \
        struct ibv_qp* TYPE ## ibv_exp_create_qp(struct ibv_context *context, struct ibv_exp_qp_init_attr *qp_init_attr) \
        { FUNC_BODY_PTR(TYPE, _EXP, ibv_exp_create_qp, lib_exp_create_qp, context, context, qp_init_attr) }
	#define HAVE_IBV_EXP_CREATE_QP_OP(OP) \
		OP(ibv_exp_create_qp)
	#define HAVE_IBV_EXP_CREATE_QP_CHECK() \
		check_api_exp(ibv_exp_create_qp, lib_exp_create_qp)
#else
	#define HAVE_IBV_EXP_CREATE_QP_FUNC(TYPE)
	#define HAVE_IBV_EXP_CREATE_QP_OP(OP)
	#define HAVE_IBV_EXP_CREATE_QP_CHECK()
#endif

#ifdef HAVE_IBV_EXP_MODIFY_QP
	#define HAVE_IBV_EXP_MODIFY_QP_FUNC(TYPE) \
        int TYPE ## ibv_exp_modify_qp(struct ibv_qp *qp, struct ibv_exp_qp_attr *attr, uint64_t attr_mask) \
        { FUNC_BODY_INT(TYPE, _EXP, ibv_exp_modify_qp, lib_exp_modify_qp, qp->context, qp, attr, attr_mask) }
	#define HAVE_IBV_EXP_MODIFY_QP_OP(OP) \
		OP(ibv_exp_modify_qp)
	#define HAVE_IBV_EXP_MODIFY_QP_CHECK() \
		check_api_exp(ibv_exp_modify_qp, lib_exp_modify_qp)
#else
	#define HAVE_IBV_EXP_MODIFY_QP_FUNC(TYPE)
	#define HAVE_IBV_EXP_MODIFY_QP_OP(OP)
	#define HAVE_IBV_EXP_MODIFY_QP_CHECK()
#endif

#ifdef HAVE_IBV_EXP_POST_SEND
	#define HAVE_IBV_EXP_POST_SEND_FUNC(TYPE) \
        int TYPE ## ibv_exp_post_send(struct ibv_qp *qp, struct ibv_exp_send_wr *wr, struct ibv_exp_send_wr **bad_wr) \
        { FUNC_BODY_INT(TYPE, _EXP, ibv_exp_post_send, drv_exp_post_send, qp->context, qp, wr, bad_wr) }
	#define HAVE_IBV_EXP_POST_SEND_OP(OP) \
		OP(ibv_exp_post_send)
	#define HAVE_IBV_EXP_POST_SEND_CHECK() \
		check_api_exp(ibv_exp_post_send, drv_exp_post_send)
#else
	#define HAVE_IBV_EXP_POST_SEND_FUNC(TYPE)
	#define HAVE_IBV_EXP_POST_SEND_OP(OP)
	#define HAVE_IBV_EXP_POST_SEND_CHECK()
#endif

#ifdef HAVE_IBV_EXP_QUERY_PORT
	#define HAVE_IBV_EXP_QUERY_PORT_FUNC(TYPE) \
        int TYPE ## ibv_exp_query_port(struct ibv_context *context, uint8_t port_num, struct ibv_exp_port_attr *port_attr) \
        { FUNC_BODY_INT(TYPE, _EXP, ibv_exp_query_port, lib_exp_query_port, context, context, port_num, port_attr) }
	#define HAVE_IBV_EXP_QUERY_PORT_OP(OP) \
		OP(ibv_exp_query_port)
	#define HAVE_IBV_EXP_QUERY_PORT_CHECK() \
		check_api_exp(ibv_exp_query_port, lib_exp_query_port)
#else
	#define HAVE_IBV_EXP_QUERY_PORT_FUNC(TYPE)
	#define HAVE_IBV_EXP_QUERY_PORT_OP(OP)
	#define HAVE_IBV_EXP_QUERY_PORT_CHECK()
#endif

#ifdef HAVE_IBV_EXP_BIND_MW
	#define HAVE_IBV_EXP_BIND_MW_FUNC(TYPE) \
        int TYPE ## ibv_exp_bind_mw(struct ibv_exp_mw_bind *mw_bind) \
        { FUNC_BODY_INT(TYPE, _EXP, ibv_exp_bind_mw, lib_exp_bind_mw, mw_bind->qp->context, mw_bind) }
	#define HAVE_IBV_EXP_BIND_MW_OP(OP) \
		OP(ibv_exp_bind_mw)
	#define HAVE_IBV_EXP_BIND_MW_CHECK() \
		check_api_exp(ibv_exp_bind_mw, lib_exp_bind_mw)
#else
	#define HAVE_IBV_EXP_BIND_MW_FUNC(TYPE)
	#define HAVE_IBV_EXP_BIND_MW_OP(OP)
	#define HAVE_IBV_EXP_BIND_MW_CHECK()
#endif

#ifdef HAVE_IBV_EXP_REG_MR
	#define HAVE_IBV_EXP_REG_MR_FUNC(TYPE) \
        struct ibv_mr* TYPE ## ibv_exp_reg_mr(struct ibv_exp_reg_mr_in *in) \
        { FUNC_BODY_PTR(TYPE, _EXP, ibv_exp_reg_mr, lib_exp_reg_mr, in->pd->context, in) }
	#define HAVE_IBV_EXP_REG_MR_OP(OP) \
		OP(ibv_exp_reg_mr)
	#define HAVE_IBV_EXP_REG_MR_CHECK() \
		check_api_exp(ibv_exp_reg_mr, lib_exp_reg_mr)
#else
	#define HAVE_IBV_EXP_REG_MR_FUNC(TYPE)
	#define HAVE_IBV_EXP_REG_MR_OP(OP)
	#define HAVE_IBV_EXP_REG_MR_CHECK()
#endif

#ifdef HAVE_IBV_EXP_CREATE_MR
	#define HAVE_IBV_EXP_CREATE_MR_FUNC(TYPE) \
        struct ibv_mr* TYPE ## ibv_exp_create_mr(struct ibv_exp_create_mr_in *in) \
        { FUNC_BODY_PTR(TYPE, _EXP, ibv_exp_create_mr, lib_exp_create_mr, in->pd->context, in) }
	#define HAVE_IBV_EXP_CREATE_MR_OP(OP) \
		OP(ibv_exp_create_mr)
	#define HAVE_IBV_EXP_CREATE_MR_CHECK() \
		check_api_exp(ibv_exp_create_mr, lib_exp_create_mr)
#else
	#define HAVE_IBV_EXP_CREATE_MR_FUNC(TYPE)
	#define HAVE_IBV_EXP_CREATE_MR_OP(OP)
	#define HAVE_IBV_EXP_CREATE_MR_CHECK()
#endif

#ifdef HAVE_IBV_EXP_CREATE_CQ_OP
	#define HAVE_IBV_EXP_CREATE_CQ_FUNC(TYPE) \
        struct ibv_cq* TYPE ## ibv_exp_create_cq(struct ibv_context *context, int cqe, void *cq_context, struct ibv_comp_channel *channel, int comp_vector, struct ibv_exp_cq_init_attr *attr) \
        { \
            void *ret, *f;                                                   \
            int flip_ret = 0;                                                \
            FUNC_BODY_RESOLVE_EXP(ibv_exp_create_cq, exp_create_cq, context) \
            PRE_##TYPE(ibv_exp_create_cq)                                    \
            ret = ((typeof(cur_ibv_ctx->item_exp.exp_create_cq))             \
                f)(context, cqe, channel, comp_vector, attr);                \
            POST_##TYPE(ibv_exp_create_cq)                                   \
            PRETEND_USED(ibv_exp_create_cq);                                 \
            PRETEND_USED(flip_ret);                                          \
            return ret;                                                      \
        }
	#define HAVE_IBV_EXP_CREATE_CQ_OP(OP) \
		OP(ibv_exp_create_cq)
	#define HAVE_IBV_EXP_CREATE_CQ_CHECK() \
		verbs_get_exp_ctx(ret)->exp_create_cq = (void*)ibv_module_context.mean.ibv_exp_create_cq
#else
	#define HAVE_IBV_EXP_CREATE_CQ_FUNC(TYPE)
	#define HAVE_IBV_EXP_CREATE_CQ_OP(OP)
	#define HAVE_IBV_EXP_CREATE_CQ_CHECK()
#endif

#ifdef HAVE_IBV_EXP_CREATE_DCT
	#define HAVE_IBV_EXP_CREATE_DCT_FUNC(TYPE) \
        struct ibv_exp_dct* TYPE ## ibv_exp_create_dct(struct ibv_context *context, struct ibv_exp_dct_init_attr *attr) \
        { FUNC_BODY_PTR(TYPE, _EXP, ibv_exp_create_dct, create_dct, context, context, attr) }
	#define HAVE_IBV_EXP_CREATE_DCT_OP(OP) \
		OP(ibv_exp_create_dct)
	#define HAVE_IBV_EXP_CREATE_DCT_CHECK() \
		check_api_exp(ibv_exp_create_dct, create_dct)
#else
	#define HAVE_IBV_EXP_CREATE_DCT_FUNC(TYPE)
	#define HAVE_IBV_EXP_CREATE_DCT_OP(OP)
	#define HAVE_IBV_EXP_CREATE_DCT_CHECK()
#endif

#ifdef HAVE_IBV_EXP_QUERY_DCT
	#define HAVE_IBV_EXP_QUERY_DCT_FUNC(TYPE) \
        int TYPE ## ibv_exp_query_dct(struct ibv_exp_dct *dct, struct ibv_exp_dct_attr *attr) \
        { FUNC_BODY_INT(TYPE, _EXP, ibv_exp_query_dct, query_dct, dct->context, dct, attr) }
	#define HAVE_IBV_EXP_QUERY_DCT_OP(OP) \
		OP(ibv_exp_query_dct)
	#define HAVE_IBV_EXP_QUERY_DCT_CHECK() \
		check_api_exp(ibv_exp_query_dct, query_dct)
#else
	#define HAVE_IBV_EXP_QUERY_DCT_FUNC(TYPE)
	#define HAVE_IBV_EXP_QUERY_DCT_OP(OP)
	#define HAVE_IBV_EXP_QUERY_DCT_CHECK()
#endif

#ifdef HAVE_IBV_EXP_DESTROY_DCT
	#define HAVE_IBV_EXP_DESTROY_DCT_FUNC(TYPE) \
        int TYPE ## ibv_exp_destroy_dct(struct ibv_exp_dct *dct) \
        { FUNC_BODY_INT(TYPE, _EXP, ibv_exp_destroy_dct, destroy_dct, dct->context, dct) }
	#define HAVE_IBV_EXP_DESTROY_DCT_OP(OP) \
		OP(ibv_exp_destroy_dct)
	#define HAVE_IBV_EXP_DESTROY_DCT_CHECK() \
		check_api_exp(ibv_exp_destroy_dct, destroy_dct)
#else
	#define HAVE_IBV_EXP_DESTROY_DCT_FUNC(TYPE)
	#define HAVE_IBV_EXP_DESTROY_DCT_OP(OP)
	#define HAVE_IBV_EXP_DESTROY_DCT_CHECK()
#endif

#ifdef HAVE_IBV_EXP_QUERY_VALUES
	#define HAVE_IBV_EXP_QUERY_VALUES_FUNC(TYPE) \
        int TYPE ## ibv_exp_query_values(struct ibv_context *context, int q_values, struct ibv_exp_values *values) \
        { FUNC_BODY_INT(TYPE, _EXP, ibv_exp_query_values, drv_exp_query_values, context, context, q_values, values) }
	#define HAVE_IBV_EXP_QUERY_VALUES_OP(OP) \
		OP(ibv_exp_query_values)
	#define HAVE_IBV_EXP_QUERY_VALUES_CHECK() \
		check_api_exp(ibv_exp_query_values, drv_exp_query_values)
#else
	#define HAVE_IBV_EXP_QUERY_VALUES_FUNC(TYPE)
	#define HAVE_IBV_EXP_QUERY_VALUES_OP(OP)
	#define HAVE_IBV_EXP_QUERY_VALUES_CHECK()
#endif

#ifdef HAVE_IBV_EXP_ARM_DCT
	#define HAVE_IBV_EXP_ARM_DCT_FUNC(TYPE) \
        int TYPE ## ibv_exp_arm_dct(struct ibv_exp_dct *dct, struct ibv_exp_arm_attr *attr) \
        { FUNC_BODY_INT(TYPE, _EXP, ibv_exp_arm_dct, lib_exp_arm_dct, dct->context, dct, attr) }
	#define HAVE_IBV_EXP_ARM_DCT_OP(OP) \
		OP(ibv_exp_arm_dct)
	#define HAVE_IBV_EXP_ARM_DCT_CHECK() \
		check_api_exp(ibv_exp_arm_dct, lib_exp_arm_dct)
#else
	#define HAVE_IBV_EXP_ARM_DCT_FUNC(TYPE)
	#define HAVE_IBV_EXP_ARM_DCT_OP(OP)
	#define HAVE_IBV_EXP_ARM_DCT_CHECK()
#endif

#ifdef HAVE_IBV_EXP_QUERY_MKEY
	#define HAVE_IBV_EXP_QUERY_MKEY_FUNC(TYPE) \
        int TYPE ## ibv_exp_query_mkey(struct ibv_mr *mr, struct ibv_exp_mkey_attr *mkey_attr) \
        { FUNC_BODY_INT(TYPE, _EXP, ibv_exp_query_mkey, lib_exp_query_mkey, mr->context, mr, mkey_attr) }
	#define HAVE_IBV_EXP_QUERY_MKEY_OP(OP) \
		OP(ibv_exp_query_mkey)
	#define HAVE_IBV_EXP_QUERY_MKEY_CHECK() \
		check_api_exp(ibv_exp_query_mkey, lib_exp_query_mkey)
#else
	#define HAVE_IBV_EXP_QUERY_MKEY_FUNC(TYPE)
	#define HAVE_IBV_EXP_QUERY_MKEY_OP(OP)
	#define HAVE_IBV_EXP_QUERY_MKEY_CHECK()
#endif

#ifdef HAVE_IBV_EXP_ALLOC_MKEY_LIST_MEMORY
#if (HAVE_IBV_EXP_ALLOC_MKEY_LIST_MEMORY == 0)
	#define HAVE_IBV_EXP_ALLOC_MKEY_LIST_MEMORY_FUNC(TYPE) \
        struct non_inline_data * TYPE ## ibv_exp_alloc_mkey_list_memory(struct ibv_exp_alloc_mkey_list_memory_in *mkey_list_memory_in) \
        { FUNC_BODY_PTR(TYPE, _EXP, ibv_exp_alloc_mkey_list_memory, lib_exp_alloc_mkey_list_memory, mkey_list_memory_in->pd->context, mkey_list_memory_in) }
#elif (HAVE_IBV_EXP_ALLOC_MKEY_LIST_MEMORY == 1)
	#define HAVE_IBV_EXP_ALLOC_MKEY_LIST_MEMORY_FUNC(TYPE) \
		struct ibv_exp_mkey_list_container * TYPE ## ibv_exp_alloc_mkey_list_memory(struct ibv_exp_mkey_list_container_attr *attr) \
		{ FUNC_BODY_PTR(TYPE, _EXP, ibv_exp_alloc_mkey_list_memory, lib_exp_alloc_mkey_list_memory, attr->pd->context, attr) }
#else
	#error HAVE_IBV_EXP_ALLOC_MKEY_LIST_MEMORY should be defined
#endif
	#define HAVE_IBV_EXP_ALLOC_MKEY_LIST_MEMORY_OP(OP) \
		OP(ibv_exp_alloc_mkey_list_memory)
	#define HAVE_IBV_EXP_ALLOC_MKEY_LIST_MEMORY_CHECK() \
		check_api_exp(ibv_exp_alloc_mkey_list_memory, lib_exp_alloc_mkey_list_memory)
#else
	#define HAVE_IBV_EXP_ALLOC_MKEY_LIST_MEMORY_FUNC(TYPE)
	#define HAVE_IBV_EXP_ALLOC_MKEY_LIST_MEMORY_OP(OP)
	#define HAVE_IBV_EXP_ALLOC_MKEY_LIST_MEMORY_CHECK()
#endif

#ifdef HAVE_IBV_EXP_DEALLOC_MKEY_LIST_MEMORY
#if (HAVE_IBV_EXP_DEALLOC_MKEY_LIST_MEMORY == 0)
	#define HAVE_IBV_EXP_DEALLOC_MKEY_LIST_MEMORY_FUNC(TYPE) \
		int TYPE ## ibv_exp_dealloc_mkey_list_memory(struct non_inline_data *mem) \
       	{ FUNC_BODY_INT(TYPE, _EXP, ibv_exp_dealloc_mkey_list_memory, lib_exp_dealloc_mkey_list_memory, mem->mr->context, mem) }
#elif  (HAVE_IBV_EXP_DEALLOC_MKEY_LIST_MEMORY == 1)
	#define HAVE_IBV_EXP_DEALLOC_MKEY_LIST_MEMORY_FUNC(TYPE) \
		int TYPE ## ibv_exp_dealloc_mkey_list_memory(struct ibv_exp_mkey_list_container *mem) \
		{ FUNC_BODY_INT(TYPE, _EXP, ibv_exp_dealloc_mkey_list_memory, lib_exp_dealloc_mkey_list_memory, mem->context, mem) }
#else
	#error HAVE_IBV_EXP_DEALLOC_MKEY_LIST_MEMORY should be defined
#endif
	#define HAVE_IBV_EXP_DEALLOC_MKEY_LIST_MEMORY_OP(OP) \
		OP(ibv_exp_dealloc_mkey_list_memory)
	#define HAVE_IBV_EXP_DEALLOC_MKEY_LIST_MEMORY_CHECK() \
		check_api_exp(ibv_exp_dealloc_mkey_list_memory, lib_exp_dealloc_mkey_list_memory)
#else
	#define HAVE_IBV_EXP_DEALLOC_MKEY_LIST_MEMORY_FUNC(TYPE)
	#define HAVE_IBV_EXP_DEALLOC_MKEY_LIST_MEMORY_OP(OP)
	#define HAVE_IBV_EXP_DEALLOC_MKEY_LIST_MEMORY_CHECK()
#endif


/* Legacy, Extended and Experimental VERBS API */
#define OP_ON_MEMBERS_LIST(OP) \
	OP(ibv_get_device_list) \
	OP(ibv_free_device_list) \
	OP(ibv_open_device) \
	OP(ibv_close_device) \
	OP(ibv_query_gid) \
	OP(ibv_create_comp_channel) \
	OP(ibv_destroy_comp_channel) \
	OP(ibv_query_device) \
	OP(ibv_query_port) \
	OP(ibv_alloc_pd) \
	OP(ibv_dealloc_pd) \
	OP(ibv_reg_mr) \
	OP(ibv_dereg_mr) \
	OP(ibv_create_cq) \
	OP(ibv_poll_cq) \
	OP(ibv_req_notify_cq) \
	OP(ibv_resize_cq) \
	OP(ibv_destroy_cq) \
	OP(ibv_create_srq) \
	OP(ibv_modify_srq) \
	OP(ibv_query_srq) \
	OP(ibv_post_srq_recv) \
	OP(ibv_create_qp) \
	OP(ibv_query_qp) \
	OP(ibv_modify_qp) \
	OP(ibv_destroy_qp) \
	OP(ibv_post_send) \
	OP(ibv_post_recv) \
	OP(ibv_create_ah) \
	OP(ibv_destroy_ah) \
	OP(ibv_attach_mcast) \
	OP(ibv_detach_mcast) \
\
	HAVE_IBV_OPEN_QP_OP(OP) \
	HAVE_IBV_CREATE_QP_EX_OP(OP) \
	HAVE_IBV_OPEN_XRCD_OP(OP) \
	HAVE_IBV_CLOSE_XRCD_OP(OP) \
\
	HAVE_IBV_EXP_QUERY_DEVICE_OP(OP) \
	HAVE_IBV_EXP_MODIFY_CQ_OP(OP) \
	HAVE_IBV_EXP_CREATE_FLOW_OP(OP) \
	HAVE_IBV_EXP_DESTROY_FLOW_OP(OP) \
	HAVE_IBV_EXP_POST_TASK_OP(OP) \
	HAVE_IBV_EXP_POLL_CQ_OP(OP) \
	HAVE_IBV_EXP_CREATE_QP_OP(OP) \
	HAVE_IBV_EXP_MODIFY_QP_OP(OP) \
	HAVE_IBV_EXP_POST_SEND_OP(OP) \
	HAVE_IBV_EXP_QUERY_PORT_OP(OP) \
	HAVE_IBV_EXP_BIND_MW_OP(OP) \
	HAVE_IBV_EXP_REG_MR_OP(OP) \
	HAVE_IBV_EXP_CREATE_MR_OP(OP) \
	HAVE_IBV_EXP_CREATE_CQ_OP(OP) \
	HAVE_IBV_EXP_CREATE_DCT_OP(OP) \
	HAVE_IBV_EXP_QUERY_DCT_OP(OP) \
	HAVE_IBV_EXP_DESTROY_DCT_OP(OP) \
	HAVE_IBV_EXP_QUERY_VALUES_OP(OP) \
	HAVE_IBV_EXP_ARM_DCT_OP(OP) \
	HAVE_IBV_EXP_QUERY_MKEY_OP(OP) \
	HAVE_IBV_EXP_ALLOC_MKEY_LIST_MEMORY_OP(OP) \
	HAVE_IBV_EXP_DEALLOC_MKEY_LIST_MEMORY_OP(OP)

/* Declare VERBS API functions to substitute original from (lib)ibverbs library */
OP_ON_MEMBERS_LIST(DECLARE_TYPE)

/* A structure to store our internal calls depending on a mode */
struct ibv_module_api_t {
	OP_ON_MEMBERS_LIST(DECLARE_STRUCT_MEMBER)
};

#define DECLARE_OPTION_STRUCT(TYPE) \
struct ibv_module_api_t ibv_##TYPE##_funcs = { \
	OP_ON_MEMBERS_LIST(PREFIX##_##TYPE) \
};

static struct module_context_t {
	struct ibv_module_api_t noble;
	struct ibv_module_api_t mean;
	struct ibv_ctx_t *ibv_ctx;
} ibv_module_context;

/*
 * How to fill the following list:
 * First, the function must be mentioned in (lib)ibverbs.
 * If its body is defined (inline) - it needs to be placed in the
 * DECLARE_OPTION_FUNCTIONS_INLINE macro. Otherwise, is should be
 * placed in DECLARE_OPTION_FUNCTIONS_PROTOTYPED, in which case it
 * will be defined by the preprocessor.
 *
 * Here's a list of arguments to be given to FUNC_BODY_(INT|VOID|PTR) macro
 * (selection is based on the type of return value of the function):
 * 1. "TYPE" - should never change (parameter of the "father-macro").
 * 2. suffix, depending on the function location (see below).
 * 3. function name - typically should start with ibv_ .
 * 4. additional name - according to the suffix (function location).
 * 5. context - pointer to ibv_context to which a function is typically bound
 * 6. list of the function's arguments.
 *
 * The tricky part is the second field, which is a suffix for the context:
 * if the function is in "struct ibv_context_ops" - use _IBV,
 * if it is in "struct verbs_context_exp" - use _EXP ,
 * and anyway add the name inside the struct,
 * otherwise (just implemented in ibverbs), use _
 *
 * Note that ibv_open_device() and ibv_close_device() get special treatment,
 * since they require an additional call to their matching libibprof handlers.
 *
 * Sanity check: "_" can't be used for the inline functions,
 * since their predecessor function would be NULL.
 */
#define DECLARE_OPTION_FUNCTIONS_PROTOTYPED(TYPE) \
        struct ibv_device** TYPE ## ibv_get_device_list(int *num_devices) \
        { FUNC_BODY_PTR(TYPE, _, ibv_get_device_list, , , num_devices) }; \
        void TYPE ## ibv_free_device_list(struct ibv_device **list) \
        { FUNC_BODY_VOID(TYPE, _, ibv_free_device_list, , , list) }; \
        struct ibv_context* TYPE ## ibv_open_device(struct ibv_device *device) \
        { \
            void* ret = NULL;                                  \
            int flip_ret = 0;                                  \
            EMPLOY_TYPE(ibv_open_device) *f;                   \
            FUNC_BODY_RESOLVE_(ibv_open_device)                \
            PRE_##TYPE(ibv_open_device)                        \
            ret = f(device);                                   \
            ibv_open_device_handler((struct ibv_context*)ret); \
            POST_RET_##TYPE(ibv_open_device)                   \
            PRETEND_USED(flip_ret);                            \
            return ret;                                        \
        }; \
        int TYPE ## ibv_close_device(struct ibv_context *context) \
        { \
            int ret;                                \
            int flip_ret = 1;                       \
            EMPLOY_TYPE(ibv_close_device) *f;       \
            ibv_close_device_handler(context);      \
            FUNC_BODY_RESOLVE_(ibv_close_device)    \
            PRE_##TYPE(ibv_close_device)            \
            ret = f(context);                       \
            POST_##TYPE(ibv_close_device)           \
            PRETEND_USED(flip_ret);                 \
            return ret;                             \
        }; \
        int TYPE ## ibv_query_device(struct ibv_context *context, struct ibv_device_attr *device_attr) \
        { FUNC_BODY_INT(TYPE, _, ibv_query_device, , context, context, device_attr) }; \
        int TYPE ## ibv_query_gid(struct ibv_context *context, uint8_t port_num, int index, union ibv_gid *gid) \
        { FUNC_BODY_INT(TYPE, _, ibv_query_gid, , , context, port_num, index, gid) }; \
        struct ibv_pd* TYPE ## ibv_alloc_pd(struct ibv_context *context) \
        { FUNC_BODY_PTR(TYPE, _, ibv_alloc_pd, alloc_pd, , context) }; \
        int TYPE ## ibv_dealloc_pd(struct ibv_pd *pd) \
        { FUNC_BODY_INT(TYPE, _, ibv_dealloc_pd, dealloc_pd, , pd) }; \
        HAVE_IBV_REG_MR_FUNC(TYPE) \
        int TYPE ## ibv_dereg_mr(struct ibv_mr *mr) \
        { FUNC_BODY_INT(TYPE, _, ibv_dereg_mr, dereg_mr, , mr) }; \
        struct ibv_comp_channel* TYPE ## ibv_create_comp_channel(struct ibv_context *context) \
        { FUNC_BODY_PTR(TYPE, _, ibv_create_comp_channel, , , context) }; \
        int TYPE ## ibv_destroy_comp_channel(struct ibv_comp_channel *channel) \
        { FUNC_BODY_INT(TYPE, _, ibv_destroy_comp_channel, , , channel) }; \
        struct ibv_cq* TYPE ## ibv_create_cq(struct ibv_context *context, int cqe, void *cq_context, struct ibv_comp_channel *channel, int comp_vector) \
        { FUNC_BODY_PTR(TYPE, _, ibv_create_cq, , , context, cqe, cq_context, channel, comp_vector) }; \
        int TYPE ## ibv_resize_cq(struct ibv_cq *cq, int cqe) \
        { FUNC_BODY_INT(TYPE, _, ibv_resize_cq, resize_cq, , cq, cqe) }; \
        int TYPE ## ibv_destroy_cq(struct ibv_cq *cq) \
        { FUNC_BODY_INT(TYPE, _, ibv_destroy_cq, destroy_cq, , cq) }; \
        struct ibv_qp* TYPE ## ibv_create_qp(struct ibv_pd *pd, struct ibv_qp_init_attr *qp_init_attr) \
        { FUNC_BODY_PTR(TYPE, _, ibv_create_qp, create_qp, , pd, qp_init_attr) }; \
        HAVE_IBV_MODIFY_QP_FUNC(TYPE) \
		HAVE_IBV_QUERY_QP_FUNC(TYPE) \
        int TYPE ## ibv_destroy_qp(struct ibv_qp *qp) \
        { FUNC_BODY_INT(TYPE, _, ibv_destroy_qp, destroy_qp, , qp) }; \
        struct ibv_ah* TYPE ## ibv_create_ah(struct ibv_pd *pd, struct ibv_ah_attr *attr) \
        { FUNC_BODY_PTR(TYPE, _, ibv_create_ah, create_ah, , pd, attr) }; \
        int TYPE ## ibv_destroy_ah(struct ibv_ah *ah) \
        { FUNC_BODY_INT(TYPE, _, ibv_destroy_ah, destroy_ah, , ah) }; \
        HAVE_IBV_ATTACH_MCAST_FUNC(TYPE) \
		HAVE_IBV_DETACH_MCAST_FUNC(TYPE) \
        struct ibv_srq* TYPE ## ibv_create_srq(struct ibv_pd *pd, struct ibv_srq_init_attr *srq_init_attr) \
        { FUNC_BODY_PTR(TYPE, _, ibv_create_srq, create_srq, , pd, srq_init_attr) }; \
        int TYPE ## ibv_modify_srq(struct ibv_srq *srq, struct ibv_srq_attr *srq_attr, int srq_attr_mask) \
        { FUNC_BODY_INT(TYPE, _, ibv_modify_srq, modify_srq, , srq, srq_attr, srq_attr_mask) }; \
        int TYPE ## ibv_query_srq(struct ibv_srq *srq, struct ibv_srq_attr *srq_attr) \
        { FUNC_BODY_INT(TYPE, _, ibv_query_srq, query_srq, , srq, srq_attr) };


#define DECLARE_OPTION_FUNCTIONS_INLINE(TYPE) \
        int TYPE ## ibv_poll_cq(struct ibv_cq *cq, int ne, struct ibv_wc *wc) \
        { FUNC_BODY_INT(TYPE, _IBV, ibv_poll_cq, poll_cq, cq->context, cq, ne, wc) }; \
        int TYPE ## ibv_post_send(struct ibv_qp *ibqp, struct ibv_send_wr *wr, struct ibv_send_wr **bad_wr) \
        { FUNC_BODY_INT(TYPE, _IBV, ibv_post_send, post_send, ibqp->context, ibqp, wr, bad_wr) }; \
        int TYPE ## ibv_post_recv(struct ibv_qp *ibqp, struct ibv_recv_wr *wr, struct ibv_recv_wr **bad_wr) \
        { FUNC_BODY_INT(TYPE, _IBV, ibv_post_recv, post_recv, ibqp->context, ibqp, wr, bad_wr) }; \
        int TYPE ## ibv_req_notify_cq(struct ibv_cq *cq, int solicited_only) \
        { FUNC_BODY_INT(TYPE, _IBV, ibv_req_notify_cq, req_notify_cq, cq->context, cq, solicited_only) }; \
        int TYPE ## ibv_post_srq_recv(struct ibv_srq *srq, struct ibv_recv_wr *recv_wr, struct ibv_recv_wr **bad_recv_wr) \
        { FUNC_BODY_INT(TYPE, _IBV, ibv_post_srq_recv, post_srq_recv, srq->context, srq, recv_wr, bad_recv_wr) }; \
        int TYPE ## ibv_query_port(struct ibv_context *context, uint8_t port_num, struct ibv_port_attr *port_attr) \
        { FUNC_BODY_INT(TYPE, _IBV, ibv_query_port, query_port, context, context, port_num, port_attr) }; \
\
		HAVE_IBV_OPEN_QP_FUNC(TYPE) \
		HAVE_IBV_CREATE_QP_EX_FUNC(TYPE) \
		HAVE_IBV_OPEN_XRCD_FUNC(TYPE) \
		HAVE_IBV_CLOSE_XRCD_FUNC(TYPE) \
\
		HAVE_IBV_EXP_QUERY_DEVICE_FUNC(TYPE) \
		HAVE_IBV_EXP_MODIFY_CQ_FUNC(TYPE) \
		HAVE_IBV_EXP_CREATE_FLOW_FUNC(TYPE) \
		HAVE_IBV_EXP_DESTROY_FLOW_FUNC(TYPE) \
		HAVE_IBV_EXP_POST_TASK_FUNC(TYPE) \
		HAVE_IBV_EXP_POLL_CQ_FUNC(TYPE) \
		HAVE_IBV_EXP_CREATE_QP_FUNC(TYPE) \
		HAVE_IBV_EXP_MODIFY_QP_FUNC(TYPE) \
		HAVE_IBV_EXP_POST_SEND_FUNC(TYPE) \
		HAVE_IBV_EXP_QUERY_PORT_FUNC(TYPE) \
		HAVE_IBV_EXP_BIND_MW_FUNC(TYPE) \
		HAVE_IBV_EXP_REG_MR_FUNC(TYPE) \
		HAVE_IBV_EXP_CREATE_MR_FUNC(TYPE) \
		HAVE_IBV_EXP_CREATE_CQ_FUNC(TYPE) \
		HAVE_IBV_EXP_CREATE_DCT_FUNC(TYPE) \
		HAVE_IBV_EXP_QUERY_DCT_FUNC(TYPE) \
		HAVE_IBV_EXP_DESTROY_DCT_FUNC(TYPE) \
		HAVE_IBV_EXP_QUERY_VALUES_FUNC(TYPE) \
		HAVE_IBV_EXP_ARM_DCT_FUNC(TYPE) \
		HAVE_IBV_EXP_QUERY_MKEY_FUNC(TYPE) \
		HAVE_IBV_EXP_ALLOC_MKEY_LIST_MEMORY_FUNC(TYPE) \
		HAVE_IBV_EXP_DEALLOC_MKEY_LIST_MEMORY_FUNC(TYPE)

#define check_dlsym(_func)  check_dlsymv(_func, DEFAULT_SYMVER)
#define check_dlsymv(_func, _ver) \
	do {                                                                \
		ibv_module_context.noble._func = sys_dlsym(#_func, _ver);   \
		if (!ibv_module_context.noble._func)                        \
			status = IBPROF_ERR_UNSUPPORTED;                    \
	} while (0)

#define check_api(_func) \
	do {                                                                \
		ret->ops._func = ibv_module_context.mean.ibv_##_func;       \
	} while (0)

#if defined(IBV_API_EXT)
#define check_api_ex(_func, call) \
	do {                                                                \
	    (verbs_get_ctx(ret))->call = ibv_module_context.mean._func;     \
	} while (0)
#endif /* IBV_API_EXT */

#if defined(IBV_API_EXT) && (IBV_API_EXT > 1)
#define check_api_exp(_func, call) \
	do {                                                                \
	    (verbs_get_exp_ctx(ret))->call = ibv_module_context.mean._func; \
	} while (0)
#endif /* IBV_API_EXT > 1 */

static inline void ibv_open_device_handler(struct ibv_context *ret)
{
	if (ret)  {
		struct ibv_ctx_t *cur_ibv_ctx = ibv_module_context.ibv_ctx;
		struct ibv_ctx_t *new_ibv_ctx = NULL;

		/* This protection is in place because this function is called
		* twice: one with the prefix and suffix of choice (e.g. profiling)
		* and once as the original function, replacing ibv_open_device() so
		* that the LD_PRELOAD replaces the original, so we can hook it.
		*/
		if (cur_ibv_ctx && ((void*)cur_ibv_ctx->addr == ret)) {
			return;
		}

		new_ibv_ctx = sys_malloc(sizeof(*cur_ibv_ctx));
		new_ibv_ctx->next = cur_ibv_ctx;
		cur_ibv_ctx = ibv_module_context.ibv_ctx = new_ibv_ctx;
		cur_ibv_ctx->addr = (uintptr_t)ret;

		/* Save original addresses of ops */
#if defined(IBV_API_EXT)
		sys_memcpy(&(cur_ibv_ctx->item), verbs_get_ctx(ret), sizeof(cur_ibv_ctx->item));
# if (IBV_API_EXT > 1)
		sys_memcpy(&(cur_ibv_ctx->item_exp), verbs_get_exp_ctx(ret), sizeof(cur_ibv_ctx->item_exp));
# endif
#else
		sys_memcpy(&(cur_ibv_ctx->item), ret, sizeof(cur_ibv_ctx->item));
#endif /* IBV_API_EXT */

		/* Replace original ops with wrappers */
		check_api(query_port);
		check_api(poll_cq);
		check_api(req_notify_cq);
		check_api(post_srq_recv);
		check_api(post_send);
		check_api(post_recv);

		/* Extended VERBS API */
		HAVE_IBV_OPEN_QP_CHECK();
		HAVE_IBV_CREATE_QP_EX_CHECK();
		HAVE_IBV_OPEN_XRCD_CHECK();
		HAVE_IBV_CLOSE_XRCD_CHECK();

		/* Experimental VERBS API*/
		HAVE_IBV_EXP_QUERY_DEVICE_CHECK();
		HAVE_IBV_EXP_MODIFY_CQ_CHECK();
		HAVE_IBV_EXP_CREATE_FLOW_CHECK();
		HAVE_IBV_EXP_DESTROY_FLOW_CHECK();
		HAVE_IBV_EXP_POST_TASK_CHECK();
		HAVE_IBV_EXP_POLL_CQ_CHECK();
		HAVE_IBV_EXP_CREATE_QP_CHECK();
		HAVE_IBV_EXP_MODIFY_QP_CHECK();
		HAVE_IBV_EXP_POST_SEND_CHECK();
		HAVE_IBV_EXP_QUERY_PORT_CHECK();
		HAVE_IBV_EXP_BIND_MW_CHECK();
		HAVE_IBV_EXP_REG_MR_CHECK();
		HAVE_IBV_EXP_CREATE_MR_CHECK();
		HAVE_IBV_EXP_CREATE_CQ_CHECK();
		HAVE_IBV_EXP_CREATE_DCT_CHECK();
		HAVE_IBV_EXP_QUERY_DCT_CHECK();
		HAVE_IBV_EXP_DESTROY_DCT_CHECK();
		HAVE_IBV_EXP_QUERY_VALUES_CHECK();
		HAVE_IBV_EXP_ARM_DCT_CHECK();
		HAVE_IBV_EXP_QUERY_MKEY_CHECK();
		HAVE_IBV_EXP_ALLOC_MKEY_LIST_MEMORY_CHECK();
		HAVE_IBV_EXP_DEALLOC_MKEY_LIST_MEMORY_CHECK();
	}
}

static inline void ibv_close_device_handler(struct ibv_context *context)
{
	if (ibv_module_context.ibv_ctx)	{
		struct ibv_ctx_t *cur_ibv_ctx = ibv_module_context.ibv_ctx;
		struct ibv_ctx_t *prev_ibv_ctx = NULL;
		while (cur_ibv_ctx && (cur_ibv_ctx->addr != (uintptr_t)context)) {
			prev_ibv_ctx = cur_ibv_ctx;
			cur_ibv_ctx = cur_ibv_ctx->next;
		}

        /* This protection is in place because this function is called
         * twice: one with the prefix and suffix of choice (e.g. profiling)
         * and once as the original function, replacing ibv_close_device() so
         * that the LD_PRELOAD replaces the original, so we can hook it.
         */
		if (!cur_ibv_ctx) {
		    return;
		}

#if defined(IBV_API_EXT)
		sys_memcpy(verbs_get_ctx(context), &(cur_ibv_ctx->item), sizeof(cur_ibv_ctx->item));
# if (IBV_API_EXT > 1)
		sys_memcpy(verbs_get_exp_ctx(context), &(cur_ibv_ctx->item_exp), sizeof(cur_ibv_ctx->item_exp));
# endif
#else
		sys_memcpy(context, &(cur_ibv_ctx->item), sizeof(cur_ibv_ctx->item));
#endif /* IBV_API_EXT */
		if (prev_ibv_ctx)
			prev_ibv_ctx->next = cur_ibv_ctx->next;
		else
			ibv_module_context.ibv_ctx = cur_ibv_ctx->next;
		sys_free(cur_ibv_ctx);
	}
}


/****************************************************************************
 * Module declaration place
 ***************************************************************************/

#define DECLARE_OPTION(type) \
DECLARE_OPTION_FUNCTIONS_PROTOTYPED(type) \
DECLARE_OPTION_FUNCTIONS_INLINE(type) \
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

static const IBPROF_MODULE_CALL ibv_tbl_call[] =
{
    OP_ON_MEMBERS_LIST(TBL_CALL_ENRTY)
	{UNDEFINED_VALUE, NULL, NULL},
};

#define check_dlsymv_IBVERBS_10(_func) check_dlsymv(_func, "IBVERBS_1.0")

static IBPROF_ERROR __ibv_init(IBPROF_MODULE_OBJECT *mod_obj)
{
	IBPROF_ERROR status = IBPROF_ERR_NONE;
    struct ibv_module_api_t* src_api = NULL;

	if ((status = sys_dlcheck("libibverbs.so")) != IBPROF_ERR_NONE)
		return status;

	/*
	 * Find the original version of functions we override.
	 */
	check_dlsym(ibv_get_device_list);
	check_dlsym(ibv_free_device_list);
	check_dlsym(ibv_open_device);
	check_dlsym(ibv_close_device);
	check_dlsym(ibv_query_device);
	check_dlsym(ibv_query_gid);
	check_dlsym(ibv_alloc_pd);
	check_dlsym(ibv_dealloc_pd);
	check_dlsym(ibv_reg_mr);
	check_dlsym(ibv_dereg_mr);
	check_dlsymv(ibv_create_comp_channel, "IBVERBS_1.0");
	check_dlsymv(ibv_destroy_comp_channel, "IBVERBS_1.0");
	check_dlsym(ibv_create_cq);
	check_dlsym(ibv_resize_cq);
	check_dlsym(ibv_destroy_cq);
        check_dlsym(ibv_create_srq);
        check_dlsym(ibv_modify_srq);
        check_dlsym(ibv_query_srq);
	check_dlsym(ibv_create_qp);
	check_dlsym(ibv_modify_qp);
	check_dlsym(ibv_query_qp);
	check_dlsym(ibv_destroy_qp);
	check_dlsym(ibv_create_ah);
	check_dlsym(ibv_destroy_ah);
	check_dlsym(ibv_attach_mcast);
	check_dlsym(ibv_detach_mcast);

	ibv_module_context.ibv_ctx = NULL;

	switch (ibprof_conf_get_int(IBPROF_MODE_IBV)) {
	case IBPROF_MODE_NONE:
		src_api = &ibv_NONE_funcs;
		break;

	case IBPROF_MODE_VERBOSE:
		src_api = &ibv_VERBOSE_funcs;
		break;

	case IBPROF_MODE_PROF:
		src_api = &ibv_PROF_funcs;
		break;

	case IBPROF_MODE_ERR:
		src_api = &ibv_ERR_funcs;
		break;

	case IBPROF_MODE_TRACE:
		src_api = &ibv_TRACE_funcs;
		break;

	default:
		src_api = &ibv_NONE_funcs;
	}

    memcpy(&ibv_module_context.mean, src_api, sizeof(*src_api));

	return status;
}

static IBPROF_ERROR __ibv_exit(IBPROF_MODULE_OBJECT *mod_obj)
{
	IBPROF_ERROR status = IBPROF_ERR_NONE;
	struct ibv_ctx_t *cur_ibv_ctx = ibv_module_context.ibv_ctx;

	while (cur_ibv_ctx) {
		struct ibv_context *context = (struct ibv_context *)cur_ibv_ctx->addr;
		ibv_module_context.ibv_ctx=cur_ibv_ctx->next;
#if defined(IBV_API_EXT)
		sys_memcpy(verbs_get_ctx(context), &(cur_ibv_ctx->item), sizeof(cur_ibv_ctx->item));
# if (IBV_API_EXT > 1)
		sys_memcpy(verbs_get_exp_ctx(context), &(cur_ibv_ctx->item_exp), sizeof(cur_ibv_ctx->item_exp));
# endif
#else
		sys_memcpy(context, &(cur_ibv_ctx->item), sizeof(cur_ibv_ctx->item));
#endif /* IBV_API_EXT */
		sys_free(cur_ibv_ctx);
		cur_ibv_ctx = ibv_module_context.ibv_ctx;
	}
	ibv_module_context.ibv_ctx = NULL;

	return status;
}

IBPROF_MODULE_OBJECT ibv_module = {
	IBPROF_MODULE_IBV,
	"libibverbs",
	"ibverbs is a library that allows programs to use RDMA \"verbs\" for " \
	"direct access to RDMA (currently InfiniBand and iWARP) hardware from " \
	"userspace.",
	ibv_tbl_call,
	__ibv_init,
	__ibv_exit,
	(void *)&ibv_module_context
};

#else
IBPROF_MODULE_OBJECT ibv_module ={ 
	IBPROF_MODULE_INVALID,
	"libibverbs",
	NULL,
	NULL,
	NULL,
	NULL,
	NULL
};
#endif /* USE_IBV */
