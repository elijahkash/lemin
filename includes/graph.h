/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   graph.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Kashnitskiy <elijahkash.code@gmail.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/09 15:15:18 by mtrisha           #+#    #+#             */
/*   Updated: 2020/01/27 17:06:12 by Kashnitskiy      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GRAPH_H
# define GRAPH_H

# include <libft.h>
# include <t_type.h>

/*
** =============================================================================
** =============================================================================
** =============================================================================
*/

/*
** value of marked_sep matter only if separate == 1
** marked_sep can be MARKED_IN or MARKED_OUT
** Look here for understanding 'separate' and 'marked_in/out' statements
** http://www.macfreek.nl/memory/Disjoint_Path_Finding
*/
typedef struct			s_node
{
	t_uint				count_connects	: 30;
	t_uint				marked			: 1;
	t_uint				in_queue		: 1;
	t_uint				separate		: 1;
	t_uint				marked_sep		: 1;
	t_uint				parent			: 30;
	t_int				weight			: 31;
	t_uint				in_new_way		: 1;
}						t_node;

# define MARKED_IN		0
# define MARKED_OUT		1

typedef struct			s_node_info
{
	t_node				*ptr;
	t_uint				self;
	t_int				weight;
}						t_node_info;

void					node_mark(t_node *node, t_int connect_state,
									t_int weight, t_uint parent);

/*
** =============================================================================
** =============================================================================
** =============================================================================
*/

typedef struct			s_connect
{
	t_uint				dst		: 30;
	t_int				state	: 2;
}						t_connect;

/*
** Since we can't delete connections (they can be reversed more then 1 times),
** we mark "deleted" ways as forbidden, and, if we pass through negative
** directions, both directions (and negative, and forbidden) turn into
** normal ways (marked as BASE_STATE).
*/
# define CONNECT_BASE_STATE	1
# define CONNECT_NEGATIVE	-1
# define CONNECT_FORBIDDEN	0

/*
** =============================================================================
** =============================================================================
** =============================================================================
*/

/*
** This struct need's for recovery way from end to start.
** So directions the same: from end to start!
** It mean's, that src_to_dst will become NEGATIVE and dst_to_src FORBIDDEN !
*/
typedef struct			s_full_connect
{
	t_node *restrict	src;
	t_node *restrict	dst;
	t_connect *restrict	src_dst;
	t_connect *restrict	dst_src;
}						t_full_connect;

void					full_connect_reverse(t_full_connect connect);

/*
** =============================================================================
** =============================================================================
** =============================================================================
*/

/*
** Some notes about graph:
** 1) size = t_farm.names.curlen !
**
** 2) here is struct of memory in *mem:
** [node_0 (count_con = i)][con_1]...[con_i],
** [node_1 (count_con = j)][con_1]...[con_j],
** ...
** [node_(size-1) (count_con = k)][con_1]...[con_k].
**
** 3) **nodes pointed to <node_0, node_1 ... node_(size-1)> in *mem.
**
** 4) We are expecting good input data and we are not deleting any nodes
** and connections, even if they don't have any profit for algorithm
** (EXCEPT loops! they are will be ignored!).
** This means that we will place even those nodes in the graph,
** that do not have any connections or belong to a connection component,
** other than the one, in which the start/end node is located.
** Also, it means, that we will place in the graph dead ends, and
** nodes, unreacheble from start (before end), but reacheble from end.
** Also, it means, that during the operation of the algorithm,
** no connection or nodes will be lost.
** Also, it means, that in case of 2 same connections,
** input ERROR will be generated !
**
** 5) Some asymptotic analysis for graph
** N - number of nodes, C - count connect for node
**  * access operation (find ptr) to node			O(1)
**  * access operation (find ptr) to connect		O(log(C))
**  * get next connect (find ptr) for node			O(1)
*/
typedef struct			s_graph
{
	t_node **restrict	nodes;
	void *restrict		mem;
	size_t				size;
	t_uint				start;
	t_uint				end;
}						t_graph;

/*
** Extremely important!!!
** add nodes STRICTLY in sorted order !
*/
void					graph_add_connect(t_graph *graph, t_uint src,
											t_uint dst);
void					graph_clear_state(t_graph *graph);
void					graph_del(t_graph *graph);

t_node					*graph_node(t_graph *graph, t_uint index);
t_connect				*graph_connect(t_node *restrict src, t_uint dst);
t_connect				*graph_node_connects(t_node *node);

/*
** =============================================================================
** =============================================================================
** =============================================================================
*/

/*
** Using an iterator is only allowed with this API:
** void 		iter_init(addres of t_iter, addres of node, traverse type);
** t_connect 	*iter_next(addres of t_iter);
**
** t_uint func - index of the necessary iteration function int array func ptrs
*/
typedef struct			s_graph_iterator
{
	t_connect *restrict	connects;
	t_uint				i;
	t_uint				count_connects;
	t_uint				func;
}						t_iter;

t_iter					*iter_init(t_iter *iter, t_node *node, t_uint type);
t_connect				*iter_next(t_iter *iter);

/*
** This is traverse type for t_iter.type
** ITER_ALL - traverse all connections of node (even forbidden);
** ITER_ALLOWED - traverse all connections except forbidden
** ITER_NEGATIVE - traverse only negative connections
** ITER_POSITIVE - traverse only positive connections
** ITER_FORBIDDEN - traverse only forbidden connections
** ITER_BY_NODE - auto chose of type, by checkig node:
** 			- if node.separate == 0		:		ITER_ALLOWED
**			- if node.marked_out == 1	:		ITER_ALLOWED
**			- if node.marked_in == 1	:		ITER_NEGATIVE
*/
# define ITER_ALL				0
# define ITER_ALLOWED			1
# define ITER_NEGATIVE			2
# define ITER_FORBIDDEN			3
# define ITER_POSITIVE			4
# define ITER_BY_NODE			5

/*
** =============================================================================
** =============================================================================
** =============================================================================
*/

#endif
