/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   farm.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtrisha <mtrisha@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/04 20:34:05 by mtrisha           #+#    #+#             */
/*   Updated: 2019/12/05 18:04:54 by mtrisha          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FARM_H
# define FARM_H

# include <libft.h>

/*
**	In fact, if graph have <= 16383 node, we can use __uint16_t !!!!!
*/
typedef __uint32_t	t_uint;

/*
** From here on will be assumed that our ant's farm can't consist from more than
** (__UINT32_MAX__ >> 2) rooms (can be written in a 30-bits number).
** Otherwise, in case ((__UINT32_MAX__ >> 2) + 1) rooms, at least 20 GB of RAM
** will be required for this programm (and this is in the case of zero
** connections).
** This trick will allow us to store each graph node in 8 bytes, and each
** connection only in 4 bytes (against 12 and 8 bytes respectively).
** This, in turn, will significantly increase efficience of this programm.
** Therefore, if in the process of reading, more then (__UINT32_MAX__ >> 2)
** rooms will be found, (even in case of sufficiency of RAM),
** input ERROR will be generated!
*/
# define MAX_NODES (__UINT32_MAX__ >> 2)

/*
** Look here for understanding 'separate' and 'marked_in/out' statements
** http://www.macfreek.nl/memory/Disjoint_Path_Finding
*/
typedef struct		s_node
{
	t_uint			count_connects : 30;
	t_uint			marked : 1;
	t_uint			separate : 1;
	t_uint			marked_in : 1;
	t_uint			marked_out : 1;
	t_uint			bfs_level : 30;
}					t_node;

typedef struct		s_connect
{
	t_uint			dst : 30;
	t_uint			state : 2;
}					t_connect;

/*
** Since we can't delete connections (thay can be reversed more then 1 times),
** we mark "deleted" ways as forbidden, and, if we pass through negative
** directions, both directions (and negative, and forbidden) turn into
** normal ways (marked as BASE_STATE).
*/
# define CONNECT_BASE_STATE	0
# define CONNECT_NEGATIVE	1
# define CONNECT_FORBIDDEN	2

/*
** 1) size = t_farm.names.curlen !
** 2) here is struct of memory in *mem:
** [node_0 (count_con = i)][con_1]...[con_i],
** [node_1 (count_con = j)][con_1]...[con_j],
** ...
** [node_(size-1) (count_con = k)][con_1]...[con_k].
** 3) **nodes pointed to <node_0, node_1 ... node_(size-1)> in *mem.
**
** Some notes about graph:
** We are expecting good input data and we are not deleting any nodes
** and connections, even if they don't have any profit for algorithm.
** This means that we will place even those nodes in the graph,
** that do not have any connections or belong to a connection component,
** other than the one, in which the start/end node is located.
** Also, it means, that we will place in the graph dead ends, and
** nodes, unreacheble from start (before end), but reacheble from end.
** Also, it means, that during the operation of the algorithm,
** no connection or nodes will be lost.
**
** Some asymptotic analysis for graph
** N - number of nodes, C - count connect for node
** 1) access operation (find ptr) to node			O(1)
** 2) access operation (find ptr) to connect		O(log(C))
** 3) get next connect (find ptr) for node			O(1)
*/
typedef struct		s_graph
{
	void			**nodes;
	void			*mem;
	size_t			size;
	t_uint			start;
	t_uint			end;
}					t_graph;

/*
** Using an iterator is only allowed with his API:
** void 		iter_init(addres of t_iter, node_nbr, traverse type, t_graph);
** t_connect 	*iter_next(addres of t_iter);
*/
typedef struct		s_graph_iterator
{
	t_uint			i;
	t_uint			node;
	t_uint			type;
}					t_iter;

/*
** This is traverse type for t_iter.type
** ITER_ALL - traverse all connections of node (even forbidden);
** ITER_ALLOWED - traverse all connections except forbidden
** ITER_NEGATIVE - traverse only negative connections
** ITER_FORBIDDEN - traverse only forbidden connections
** ITER_BY_SEPARATE_NODE - auto chose of type, by checkig node:
** 			- if node.separate == 0		:		ITER_ALLOWED
**			- if node.marked_out == 1	:		ITER_ALLOWED
**			- if node.marked_in == 1	:		ITER_NEGATIVE
*/
# define ITER_ALL				1
# define ITER_ALLOWED			2
# define ITER_NEGATIVE			4
# define ITER_FORBIDDEN			8
# define ITER_BY_SEPARATE_NODE	16

/*
** Note about input:
** as coordinates of each node dost not have no effect on
** the decision algorithm, we dont save them at all!!!!!
**
** Here is description of sense t_vect 'chars'; t_vect 'names'.
** t_vect - this is dynamic array from libft.h.
** 'chars' - array of chars
** 'names' - array of indexes/ptr. Each index/ptr point, where in 'chars'
** located his C-string.
** This trick let us save up to 3 bytes per string +
** allows us to achieve an extremely high level of cacheability while
** output of graph node names +
** in the process of finding the paths, we will operate with structures with
** information of the minimum length that we exclusively need.
** access to the names of the graph we need only to display
** the already found paths.
**
** That is the point:
** # when we read new node (C-string arbitrary lenght <l>)
** 		- add to 'chars' <l + 1> symbols : full string + '\0'
**  	- add to 'names' indes of first letter this string in 'chars'
** # when all nodes have been read:
**		- trim extra mem in 'chars' and 'names'
**		- instead of indexes in names write direct pointers
** # relax :)
*/
typedef struct		s_farm
{
	long long		ants;
	t_vect			chars;
	t_vect			names;
	t_graph			graph;
}					t_farm;

typedef struct		s_full_connect
{
	t_connect		src_to_dst;
	t_connect		dst_to_src;
}					t_full_connect;

/*
**	ants - this is the number of ants that will go this way.
*/
typedef struct		s_way
{
	t_uint			*nodes;
	t_uint			len;
	t_uint			border; // no need!!!!!!!
	long long		ants;
}					t_way;

typedef struct		s_enum_ways
{
	t_way			*ways;
	t_uint			count;
}					t_enum_ways;

#endif