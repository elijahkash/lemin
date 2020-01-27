/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   farm.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Kashnitskiy <elijahkash.code@gmail.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/04 20:34:05 by mtrisha           #+#    #+#             */
/*   Updated: 2020/01/27 17:06:14 by Kashnitskiy      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FARM_H
# define FARM_H

# include <libft.h>

# include <t_type.h>
# include <graph.h>

/*
** From here on will be assumed that our ant's farm can't consist from more than
** (__UINT32_MAX__ >> 2) rooms (can be written in a 30-bits number).
** Otherwise, in case ((__UINT32_MAX__ >> 2) + 1) rooms, at least 20 GB of RAM
** will be required for this programm (and this is in the case of zero
** connections).
** This trick will allow us to store each graph node in 12 bytes, and each
** connection only in 4 bytes (against 16 and 8 bytes respectively).
** This, in turn, will significantly increase efficience of this programm.
** Therefore, if in the process of reading, more then (__UINT32_MAX__ >> 2)
** rooms will be found, (even in case of sufficiency of RAM),
** input ERROR will be generated!
*/
# define MAX_NODES (__UINT32_MAX__ >> 2)

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
** start/end stored positions of start and end in 'chars'
*/
typedef struct			s_farm
{
	t_uint				ants;
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
** return (SAME_WAYS) if find; return (0) otherwise
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

#endif
