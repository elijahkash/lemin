/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   farm.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtrisha <mtrisha@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/04 20:34:05 by mtrisha           #+#    #+#             */
/*   Updated: 2019/12/04 21:49:56 by mtrisha          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FARM_H
# define FARM_H

# include <libft.h>

typedef __uint32_t	uint;

typedef struct		s_node
{
	uint			connects : 30;
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
