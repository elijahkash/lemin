/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   farm.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtrisha <mtrisha@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/04 20:34:05 by mtrisha           #+#    #+#             */
/*   Updated: 2019/12/05 15:11:27 by mtrisha          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FARM_H
# define FARM_H

# include <libft.h>

typedef __uint32_t	uint;

/*
** From here on will be assume that in our ant's farm can't consist more than
** (__UINT32_MAX__ >> 2) rooms (can be written in a 30-bits number).
** Otherwise, in case (__UINT32_MAX__ >> 2) + 1 rooms, at least 20 GB of RAM
** will be required for this programm (and this is in the case of zero
** connections).
** This trick will allow us to store each graph node in 8 bytes, and each
** connection only in 4 bytes (against 12 and 8 bytes respectively).
** This, in turn, will significantly increase efficience of this programm.
** Therefore, if in the process of reading, more then (__UINT32_MAX__ >> 2)
** rooms will be found, (even in case of sufficiency RAM),
** input ERROR will be generated!
*/
# define MAX_NODES (__UINT32_MAX__ >> 2)

/*
** Look here for understanding 'separate' and 'marked_in/out' statements
** http://www.macfreek.nl/memory/Disjoint_Path_Finding
*/
typedef struct		s_node
{
	uint			count_connects : 30;
	uint			marked : 1;
	uint			separate : 1;
	uint			marked_in : 1;
	uint			marked_out : 1;
	uint			bfs_level : 30;
}					t_node;

typedef struct		s_connect
{
	uint			dst : 30;
	uint			state : 2;
}					t_connect;

/*
** Since we can't delete connections (thay can be reversed more then 1 times),
** we mark "deleted" ways as forbidden, and, if we pass through negative
** directions, both directions (and negative, and forbidden) turning into
** normal ways (marked as BASE_STATE).
*/
# define CONNECT_BASE_STATE	0
# define CONNECT_NEGATIVE	1
# define CONNECT_FORBIDDEN	2

/*
**
*/
typedef struct		s_graph
{
	void			*mem;
	size_t			size;
	uint			start;
	uint			end;
}					t_graph;

typedef struct		s_graph_iterator
{
	uint			i;
	uint			node;
	uint			type;
}					t_iter;

# define ITER_ALL				1
# define ITER_ALLOWED			2
# define ITER_NEGATIVE			4
# define ITER_FORBIDDEN			8
# define ITER_BY_SEPARATE_NODE	16

typedef struct		s_farm
{
	long long		ants;
	t_vect			chars;
	t_vect			names;
	t_graph			graph;
}					t_farm;

typedef struct		s_full_connect
{
	t_connect		src;
	t_connect		dst;
}					t_full_connect;

typedef struct		s_way
{
	uint			*nodes;
	uint			len;
	uint			border; // no need!!!!!!!
	long long		ants;
}					t_way;

typedef struct		s_enum_ways
{
	t_way			*ways;
	uint			count;
}					t_enum_ways;

#endif
