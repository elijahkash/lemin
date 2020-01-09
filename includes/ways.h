/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ways.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtrisha <mtrisha@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/09 15:00:02 by mtrisha           #+#    #+#             */
/*   Updated: 2020/01/09 15:02:46 by mtrisha          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WAYS_H
# define WAYS_H

# include <farm.h>

typedef struct			s_way
{
	t_uint *restrict	nodes;
	t_uint *restrict	ants_names;
	t_uint				len;
	t_uint				ants;
	t_int				first_ant;
	t_uint				ants_on_way;
}						t_way;

void					way_init(t_way *way, t_uint *arr, t_uint len);
int						comp_way_by_len(const void *way1, const void *way2);

/*
** =============================================================================
*/

typedef struct			s_enum_ways
{
	t_way *restrict		ways;
	t_uint *restrict	nodes_mem;
	t_uint *restrict	ants_mem;
	t_uint				count;
	t_uint				moves;
}						t_enum_ways;

void					enum_ways_init(t_enum_ways *combs, t_uint count);
void					enum_ways_del(t_enum_ways *combs);

/*
** for main idea look here (in bottom):
** https://github.com/VBrazhnik/Lem_in/wiki/Algorithm
*/
t_uint					count_moves(t_enum_ways *combs, t_uint ants);

void					print_result(t_enum_ways *combs, t_farm *farm);

#endif
