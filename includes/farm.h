/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   farm.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtrisha <mtrisha@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/04 20:34:05 by mtrisha           #+#    #+#             */
/*   Updated: 2019/12/21 20:52:41 by mtrisha          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FARM_H
# define FARM_H

# include <libft.h>

/*
** //TODO: restrict inline macros
*/

/*
**	In fact, if graph have <= 16383 node, we can use __uint16_t !!!!!
*/
typedef __uint32_t		t_uint;
typedef __int32_t		t_int;

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
	t_uint				count_connects : 30;
	t_uint				marked : 1;
	t_uint				in_deq : 1;
	t_uint				separate : 1;
	t_uint				marked_sep : 1;
	t_uint				parent : 30;
	t_int				weight : 31;
}						t_node;

# define MARKED_IN		0
# define MARKED_OUT		1

typedef struct			s_node_info
{
	t_uint				self;
	t_int				weight;
}						t_node_info;

void					node_mark(t_node *node, t_uint connect_state,
									t_int weight, t_uint parent);

/*
** =============================================================================
** =============================================================================
** =============================================================================
*/

typedef struct			s_connect
{
	t_uint				dst : 30;
	t_uint				state : 2;
}						t_connect;

/*
** Since we can't delete connections (they can be reversed more then 1 times),
** we mark "deleted" ways as forbidden, and, if we pass through negative
** directions, both directions (and negative, and forbidden) turn into
** normal ways (marked as BASE_STATE).
*/
# define CONNECT_BASE_STATE	0
# define CONNECT_NEGATIVE	1
# define CONNECT_FORBIDDEN	2

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
	t_connect *restrict	src_to_dst;
	t_connect *restrict	dst_to_src;
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

void					graph_del(t_graph *graph);

/*
** Extremely important!!!
** add nodes STRICTLY in sorted order !
*/
void					graph_add_connect(t_graph *graph, t_uint src,
											t_uint dst);
t_node					*graph_node(t_graph *graph, t_uint index);
t_connect				*graph_connect(t_node *restrict src, t_uint dst);
t_connect				*graph_node_connects(t_node *node);
void					graph_clear_state(t_graph *graph);

/*
** =============================================================================
** =============================================================================
** =============================================================================
*/

/*
** Using an iterator is only allowed with his API:
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
# define ITER_BY_NODE			4

/*
** =============================================================================
** =============================================================================
** =============================================================================
*/

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
**		- sort 'names' by strcmp (for some optimisations - use head!)
** # relax :)
**
** Note about reading connects:
** We can't create t_graph before reading all rooms.
** It have difficult structure, so first we consider all the connections
** (in t_vect connects), trim mem, sort them by src node, count
** count_connections for every node,
** THEN create t_graph, and after this, del t_vect connections for memory save
** (because at this moment, all connections stored in t_graph).
**
** start/end stored positions of start end end in 'chars'
*/
typedef struct			s_farm
{
	long long			ants;
	t_vect				chars;
	t_vect				names;
	t_vect				connects;
	t_graph				graph;
	size_t				start;
	size_t				end;
}						t_farm;

# define FARM_INIT_SE_VALUES			((size_t)(0 - 1))

# define FARM_INIT_ROOM_COUNT			1024
# define FARM_INIT_CHARS_PER_ROOM		8
# define FARM_INIT_CHARS_COUNT (FARM_INIT_ROOM_COUNT * FARM_INIT_CHARS_PER_ROOM)
# define FARM_INIT_CONNECTS_PER_ROOM	2.5

void					farm_init(t_farm *restrict farm);
void					farm_del(t_farm *restrict farm);
void					farm_init_rooms(t_farm *farm);
void					farm_init_connects(t_farm *farm);
void					farm_del_connects(t_farm *farm);
void					farm_del_rooms(t_farm *farm);

/*
** For fast, check on same connection here.
** return (1) if find; return (0) otherwise
*/
int						graph_init(t_graph *graph, t_farm *farm);

/*
** =============================================================================
** =============================================================================
** =============================================================================
*/

/*
** Used for stored input data (connections) in farm.connects
*/
typedef struct			s_double_number
{
	t_uint				a;
	t_uint				b;
}						t_dnbr;

int						dnbr_cmp(const void *number_1, const void *number_2);

/*
** =============================================================================
** =============================================================================
** =============================================================================
*/

/*
**	ants - this is the number of ants that will go this way.
** //TODO: type ants_names
*/
typedef struct			s_way
{
	t_uint *restrict	nodes;
	int					*ants_names;
	int					last_ant;
	int					first_ant;
	t_uint				len;
	long long			ants;
}						t_way;

void					way_init(t_way *way, t_uint *arr, t_uint len);
void					way_del(t_way way);
int						comp_way_by_len(const void *way1, const void *way2);

/*
** =============================================================================
** =============================================================================
** =============================================================================
*/

typedef struct			s_enum_ways
{
	t_way *restrict		ways;
	t_uint *restrict	nodes_mem;
	t_uint				count;
	long long			moves;
}						t_enum_ways;

void					enum_ways_init(t_enum_ways *combs, t_uint count);
void					enum_ways_del(t_enum_ways *combs);
/*
** for main idea look here (in bottom):
** https://github.com/VBrazhnik/Lem_in/wiki/Algorithm
*/
long long				count_moves(t_enum_ways *combs, long long ants);

void					print_result(t_enum_ways *combs, t_farm *farm);

#endif
