/*
 * Copyright (c) 2013-2015 Mellanox Technologies, Inc.
 *                         All rights reserved.
 * $COPYRIGHT$
 *
 * Additional copyrights may follow
 *
 * $HEADER$
 */

#ifndef _IBPROF_IO_H_
#define _IBPROF_IO_H_

/**
 * ibprof_plain_dump
 *
 * @brief
 *    Dumps all gathered information in plain.
 *
 * @return @a none
 ***************************************************************************/
void ibprof_io_plain_dump(FILE* file, IBPROF_OBJECT *ibprof_obj);

/**
 * ibprof_xml_dump
 *
 * @brief
 *    Dumps all gathered information in xml format.
 *
 * @return @a none
 ***************************************************************************/
void ibprof_io_xml_dump(FILE* file, IBPROF_OBJECT *task_obj);

#endif /* _IBPROF_IO_H_ */
