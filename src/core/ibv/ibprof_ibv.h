/*
 * Copyright (c) 2013-2015 Mellanox Technologies, Inc.
 *                         All rights reserved.
 * $COPYRIGHT$
 *
 * Additional copyrights may follow
 *
 * $HEADER$
 */

/*
 * This file is used to generate the function stubs for ibprof.
 * Each suffix (e.g. NONE, PROF, ERR, etc.) signifies a run-time
 * option for ibprof callbacks. In order to add a new option,
 * all that is required is to add the following three macros:
 *
 * #define PRE_SUFFIX
 * - what to do before the original is called
 * #define POST_SUFFIX(func_name)
 * - what to do after the original is called (w/o a return value)
 * #define POST_RET_SUFFIX(func_name)
 * - what to do after the original is called (return value is "ret")
 *
 * Also, need to add a single line using this macro in the .c file.
 */

#ifndef offsetof
#define offsetof(TYPE, MEMBER) ((uintptr_t) &((TYPE *)0)->MEMBER)
#endif

#define PRETEND_USED(var) do { (void)(var); } while (0)

/* Mock mode - do nothing (can be used to compare run-time against no-ibprof runs) */
#define PRE_NONE(func_name)
#define POST_NONE(func_name)
#define POST_RET_NONE(func_name)

/* Verbose mode - output the name of the functions entered and left */
#define PRE_VERBOSE(func_name) IBPROF_TRACE("IN %s:%s\n", __FILE__, __FUNCTION__);
#define POST_VERBOSE(func_name) PRETEND_USED(flip_ret); \
    IBPROF_TRACE("OUT %s:%s\n", __FILE__, __FUNCTION__);
#define POST_RET_VERBOSE(func_name) PRETEND_USED(flip_ret); \
    IBPROF_TRACE("OUT %s:%s\n", __FILE__, __FUNCTION__);

/* Profiling mode - collect timing information about function run-times */
#define PRE_PROF(func_name) \
	double tm_start; \
	tm_start = ibprof_timestamp();
#define POST_PROF(func_name) \
	ibprof_update(IBPROF_MODULE_IBV, TBL_CALL_NUMBER(func_name), \
            ibprof_timestamp_diff(tm_start));
#define POST_RET_PROF(func_name) \
	ibprof_update(IBPROF_MODULE_IBV, TBL_CALL_NUMBER(func_name), \
            ibprof_timestamp_diff(tm_start));

/* Error-injection mode - return an error with some probability */
#define PRE_ERR(func_name) \
	double tm_start; \
	int64_t err = 0; \
	tm_start = ibprof_timestamp();
#define POST_ERR(func_name) \
	ibprof_update_ex(IBPROF_MODULE_IBV, TBL_CALL_NUMBER(func_name), \
            ibprof_timestamp_diff(tm_start), &err);
#define POST_RET_ERR(func_name) \
	if ((rand() % 100) < ibprof_conf_get_int(IBPROF_ERR_PERCENT)) ret = (flip_ret ? ((typeof(ret))1) : ((typeof(ret))0)); \
	err = (flip_ret ? (ret != 0) : (ret == 0)); \
	ibprof_update_ex(IBPROF_MODULE_IBV, TBL_CALL_NUMBER(func_name), \
            ibprof_timestamp_diff(tm_start), &err);

/* TODO: Stack-tracing mode - collect information about the origin of function calls */
#define PRE_TRACE(func_name)
#define POST_TRACE(func_name)
#define POST_RET_TRACE(func_name)

/*
 * Common macros, presenting the function stubs
 */
#define PRE_(func_name) f = ibv_module_context.mean.func_name;
#define POST_(func_name)
#define POST_RET_(func_name)

#define FUNC_BODY_RESOLVE_GET_CTX(context) ({                           \
    struct ibv_ctx_t *cur_ibv_ctx = ibv_module_context.ibv_ctx;         \
        while ((cur_ibv_ctx->addr != (uintptr_t)context) &&             \
            cur_ibv_ctx->next)                                          \
            cur_ibv_ctx = cur_ibv_ctx->next;                            \
    cur_ibv_ctx;                                                        \
    })

#define FUNC_BODY_RESOLVE_(func_name, ...)                              \
    f = ibv_module_context.noble.func_name;

#if defined(IBV_API_EXT)
#define FUNC_BODY_RESOLVE_IBV(func_name, ex_name, ctx)                  \
    struct ibv_ctx_t *cur_ibv_ctx = FUNC_BODY_RESOLVE_GET_CTX(ctx);     \
    f = cur_ibv_ctx->item.context.ops.ex_name;
#else
#define FUNC_BODY_RESOLVE_IBV(func_name, ex_name, ctx)                  \
    struct ibv_ctx_t *cur_ibv_ctx = FUNC_BODY_RESOLVE_GET_CTX(ctx);     \
    f = cur_ibv_ctx->item.ops.ex_name;
#endif

#if defined(IBV_API_EXT)
#define FUNC_BODY_RESOLVE_EX(func_name, ex_name, ctx)                   \
    struct ibv_ctx_t *cur_ibv_ctx = FUNC_BODY_RESOLVE_GET_CTX(ctx);     \
    f = cur_ibv_ctx->item.ex_name;
#else
#define FUNC_BODY_RESOLVE_EX(func_name, ex_name, ctx)                   \
        struct ibv_ctx_t *cur_ibv_ctx = NULL;                           \
        PRETEND_USED(cur_ibv_ctx);                                      \
        f = NULL;
#endif

# if (IBV_API_EXT > 1)
#define FUNC_BODY_RESOLVE_EXP(func_name, ex_name, ctx)                  \
    struct ibv_ctx_t *cur_ibv_ctx = FUNC_BODY_RESOLVE_GET_CTX(ctx);     \
    f = cur_ibv_ctx->item_exp.ex_name;
#else
#define FUNC_BODY_RESOLVE_EXP(func_name, ex_name, ctx)                  \
    struct ibv_ctx_t *cur_ibv_ctx = NULL;                               \
    PRETEND_USED(cur_ibv_ctx);                                          \
    f = NULL;
#endif

#define FUNC_BODY_INT(type, ctx_type, func_name, ex_name, ctx, ...)     \
    int ret;                                                            \
    int flip_ret = 1;                                                   \
    EMPLOY_TYPE(func_name) *f;                                          \
    FUNC_BODY_RESOLVE##ctx_type(func_name, ex_name, ctx)                \
    PRE_##type(func_name)                                               \
    ret = f(__VA_ARGS__);                                               \
    POST_RET_##type(func_name)                                          \
    PRETEND_USED(flip_ret);                                             \
    return ret;

#define FUNC_BODY_VOID(type, ctx_type, func_name, ex_name, ctx, ...)    \
    int flip_ret = 0;                                                   \
    EMPLOY_TYPE(func_name) *f;                                          \
    FUNC_BODY_RESOLVE##ctx_type(func_name, ex_name, ctx)                \
    PRE_##type(func_name)                                               \
    f(__VA_ARGS__);                                                     \
    POST_##type(func_name)                                              \
    PRETEND_USED(flip_ret);

#define FUNC_BODY_PTR(type, ctx_type, func_name, ex_name, ctx, ...)     \
    void* ret;                                                          \
    int flip_ret = 0;                                                   \
    EMPLOY_TYPE(func_name) *f;                                          \
    FUNC_BODY_RESOLVE##ctx_type(func_name, ex_name, ctx)                \
    PRE_##type(func_name)                                               \
    ret = f(__VA_ARGS__);                                               \
    POST_RET_##type(func_name)                                          \
    PRETEND_USED(flip_ret);                                             \
    return ret;

#define EMPLOY_TYPE(func_name) __type_of_##func_name
#define DECLARE_TYPE(func_name) \
        typedef typeof(func_name) EMPLOY_TYPE(func_name);
#define DECLARE_STRUCT_MEMBER(func_name) \
        EMPLOY_TYPE(func_name) * func_name;
#define TBL_CALL_NUMBER(func_name) \
        offsetof(struct ibv_module_api_t, func_name) / sizeof(void*)
#define TBL_CALL_ENRTY(func_name) \
        { TBL_CALL_NUMBER(func_name), #func_name, NULL},
