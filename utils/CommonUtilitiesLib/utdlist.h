/**************************************************************************/
/*                                                                        */
/* Copyright (c) 2005 Tranzda Technologies Co.,Ltd.                       */
/*                    深圳市全智达科技有限公司  版权所有 2005             */
/*                                                                        */
/*  PROPRIETARY RIGHTS of Tranzda Technologies Co.,Ltd. are involved in   */
/*  the subject matter of this material. All manufacturing, reproduction, */
/*  use, and sales rights pertaining to this subject matter are governed  */
/*  by the license agreement.  The recipient of this software implicitly  */ 
/*  accepts the terms of the license.                                     */
/*  本软件文档资料是深圳市全智达科技有限公司的合法资产，任何人士阅读和使  */
/*  用本资料必须获得相应的书面授权,承担保密责任和接受相应的法律约束。     */
/*                                                                        */
/**************************************************************************/

/**********************************************************************
List utilities

(c) 1995 Innobase Oy

Created 9/10/1995 Heikki Tuuri
***********************************************************************/

#ifndef ut0lst_h
#define ut0lst_h

//#include "univ.i"
#define ut_ad(exp)
#define ut_a(exp)

/* This module implements the two-way linear list which should be used
if a list is used in the database. Note that a single struct may belong
to two or more lists, provided that the list are given different names.
An example of the usage of the lists can be found in fil0fil.c. */

/***********************************************************************
This macro expands to the unnamed type definition of a struct which acts
as the two-way list base node. The base node contains pointers
to both ends of the list and a count of nodes in the list (excluding
the base node from the count). TYPE should be the list node type name. */

#define UT_LIST_BASE_NODE_T(TYPE)\
struct {\
	int	count;	/* count of nodes in list */\
	TYPE *	start;	/* pointer to list start, NULL if empty */\
	TYPE *	end;	/* pointer to list end, NULL if empty */\
}\

/***********************************************************************
This macro expands to the unnamed type definition of a struct which
should be embedded in the nodes of the list, the node type must be a struct.
This struct contains the pointers to next and previous nodes in the list.
The name of the field in the node struct should be the name given
to the list. TYPE should be the list node type name. Example of usage:

typedef struct LRU_node_struct	LRU_node_t;
struct LRU_node_struct {
	UT_LIST_NODE_T(LRU_node_t)	LRU_list;
	...
}
The example implements an LRU list of name LRU_list. Its nodes are of type
LRU_node_t.
*/

#define UT_LIST_NODE_T(TYPE)\
struct {\
	TYPE *	prev;	/* pointer to the previous node,\
			NULL if start of list */\
	TYPE *	next;	/* pointer to next node, NULL if end of list */\
}\

/***********************************************************************
Initializes the base node of a two-way list. */

#define UT_LIST_INIT(BASE)\
{\
	(BASE).count = 0;\
	(BASE).start = NULL;\
	(BASE).end   = NULL;\
}\

/***********************************************************************
Adds the node as the first element in a two-way linked list.
BASE has to be the base node (not a pointer to it). N has to be
the pointer to the node to be added to the list. NAME is the list name. */

#define UT_LIST_ADD_FIRST(NAME, BASE, N)\
{\
	ut_ad(N);\
	((BASE).count)++;\
	((N)->NAME).next = (BASE).start;\
	((N)->NAME).prev = NULL;\
	if ((BASE).start != NULL) {\
		(((BASE).start)->NAME).prev = (N);\
	}\
	(BASE).start = (N);\
	if ((BASE).end == NULL) {\
		(BASE).end = (N);\
	}\
}\

/***********************************************************************
Adds the node as the last element in a two-way linked list.
BASE has to be the base node (not a pointer to it). N has to be
the pointer to the node to be added to the list. NAME is the list name. */

#define UT_LIST_ADD_LAST(NAME, BASE, N)\
{\
	ut_ad(N);\
	((BASE).count)++;\
	((N)->NAME).prev = (BASE).end;\
	((N)->NAME).next = NULL;\
	if ((BASE).end != NULL) {\
		(((BASE).end)->NAME).next = (N);\
	}\
	(BASE).end = (N);\
	if ((BASE).start == NULL) {\
		(BASE).start = (N);\
	}\
}\

/***********************************************************************
Inserts a NODE2 after NODE1 in a list.
BASE has to be the base node (not a pointer to it). NAME is the list
name, NODE1 and NODE2 are pointers to nodes. */

#define UT_LIST_INSERT_AFTER(NAME, BASE, NODE1, NODE2)\
{\
	ut_ad(NODE1);\
	ut_ad(NODE2);\
	((BASE).count)++;\
	((NODE2)->NAME).prev = (NODE1);\
	((NODE2)->NAME).next = ((NODE1)->NAME).next;\
	if (((NODE1)->NAME).next != NULL) {\
		((((NODE1)->NAME).next)->NAME).prev = (NODE2);\
	}\
	((NODE1)->NAME).next = (NODE2);\
	if ((BASE).end == (NODE1)) {\
		(BASE).end = (NODE2);\
	}\
}\


#define UT_LIST_INSERT_BEFORE(NAME, BASE, NODE1, NODE2)\
{\
    ut_ad(NODE1);\
    ut_ad(NODE2);\
    ((BASE).count)++;\
    ((NODE2)->NAME).next = (NODE1);\
    ((NODE2)->NAME).prev = ((NODE1)->NAME).prev;\
    if (((NODE1)->NAME).prev != NULL) {\
    ((((NODE1)->NAME).prev)->NAME).next = (NODE2);\
    }\
    ((NODE1)->NAME).prev = (NODE2);\
    if ((BASE).start == (NODE1)) {\
    (BASE).start = (NODE2);\
    }\
}\

/***********************************************************************
Removes a node from a two-way linked list. BASE has to be the base node
(not a pointer to it). N has to be the pointer to the node to be removed
from the list. NAME is the list name. */

#define UT_LIST_REMOVE(NAME, BASE, N)\
{\
	if(((N)->NAME).next !=NULL || ((N)->NAME).prev != NULL){\
	ut_ad(N);\
	ut_a((BASE).count > 0);\
	((BASE).count)--;\
	if (((N)->NAME).next != NULL) {\
		((((N)->NAME).next)->NAME).prev = ((N)->NAME).prev;\
	} else {\
		(BASE).end = ((N)->NAME).prev;\
	}\
	if (((N)->NAME).prev != NULL) {\
		((((N)->NAME).prev)->NAME).next = ((N)->NAME).next;\
	} else {\
		(BASE).start = ((N)->NAME).next;\
	}\
	((N)->NAME).next = ((N)->NAME).prev = NULL;\
	}\
	else{\
		if((BASE).start == (N) && (BASE).count == 1){\
			(BASE).start = (BASE).end = NULL;\
			(BASE).count = 0;\
		}\
	}\
}
	//((N)->NAME).next = ((N)->NAME).prev = NULL;
		//if(((N)->NAME).next !=NULL || ((N)->NAME).prev != NULL;\)
/************************************************************************
Gets the next node in a two-way list. NAME is the name of the list
and N is pointer to a node. */

#define UT_LIST_GET_NEXT(NAME, N)\
	(((N)->NAME).next)

/************************************************************************
Gets the previous node in a two-way list. NAME is the name of the list
and N is pointer to a node. */

#define UT_LIST_GET_PREV(NAME, N)\
	(((N)->NAME).prev)

/************************************************************************
Alternative macro to get the number of nodes in a two-way list, i.e.,
its length. BASE is the base node (not a pointer to it). */

#define UT_LIST_GET_LEN(BASE)\
	(BASE).count

/************************************************************************
Gets the first node in a two-way list, or returns NULL,
if the list is empty. BASE is the base node (not a pointer to it). */

#define UT_LIST_GET_FIRST(BASE)\
	(BASE).start

/************************************************************************
Gets the last node in a two-way list, or returns NULL,
if the list is empty. BASE is the base node (not a pointer to it). */

#define UT_LIST_GET_LAST(BASE)\
	(BASE).end

/************************************************************************
Checks the consistency of a two-way list. NAME is the name of the list,
TYPE is the node type, and BASE is the base node (not a pointer to it). */

#define UT_LIST_VALIDATE(NAME, TYPE, BASE)\
{\
	ulint	ut_list_i_313;\
	TYPE *	ut_list_node_313;\
\
	ut_list_node_313 = (BASE).start;\
\
	for (ut_list_i_313 = 0; ut_list_i_313 < (BASE).count;\
	 					ut_list_i_313++) {\
	 	ut_a(ut_list_node_313);\
	 	ut_list_node_313 = (ut_list_node_313->NAME).next;\
	}\
\
	ut_a(ut_list_node_313 == NULL);\
\
	ut_list_node_313 = (BASE).end;\
\
	for (ut_list_i_313 = 0; ut_list_i_313 < (BASE).count;\
	 					ut_list_i_313++) {\
	 	ut_a(ut_list_node_313);\
	 	ut_list_node_313 = (ut_list_node_313->NAME).prev;\
	}\
\
	ut_a(ut_list_node_313 == NULL);\
}\

////定义了一个通用的队列和队列头。其中，data用来存放的实际的节点数据。
//struct Stru_ListNode;
//typedef struct Stru_ListNode ListNodeDataType;
//struct Stru_ListNode{
//	UT_LIST_NODE_T(ListNodeDataType) listNode;
//	UInt32 id;//用来提供节点标志，也可以不用
//	void * data;
//};

//#define ListHeadDataType UT_LIST_BASE_NODE_T(ListNodeDataType) 

#endif

