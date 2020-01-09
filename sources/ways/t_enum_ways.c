/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   t_enum_ways.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtrisha <mtrisha@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/09 15:03:27 by mtrisha           #+#    #+#             */
/*   Updated: 2020/01/09 15:03:59 by mtrisha          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ways.h>

void					enum_ways_init(t_enum_ways *restrict combs,
										t_uint count)
{
	combs->moves = 0;
	combs->count = count;
	combs->nodes_mem = NULL;
	combs->ants_mem = NULL;
	combs->ways = (t_way *)ft_malloc(sizeof(t_way) * count);
}

void					enum_ways_del(t_enum_ways *restrict combs)
{
	if (combs->nodes_mem)
		ft_free(combs->nodes_mem);
	if (combs->ways)
		ft_free(combs->ways);
	if (combs->ants_mem)
		ft_free(combs->ants_mem);
	combs->moves = 0;
	combs->count = 0;
	combs->ants_mem = NULL;
	combs->nodes_mem = NULL;
	combs->ways = NULL;
}

static void				fill_borders(t_uint *restrict borders,
										t_enum_ways *restrict combs)
{
	t_uint	i;
	t_uint	j;
	t_uint	len;

	len = combs->count;
	i = 0;
	while (i < len)
	{
		borders[i] = 0;
		j = 0;
		while (j < i)
		{
			borders[i] += (combs->ways[i].len - combs->ways[j].len);
			j++;
		}
		i++;
	}
}

t_uint					count_moves(t_enum_ways *restrict combs, t_uint ants)
{
	t_uint		cur_count;
	t_uint		tmp;
	t_uint		way;
	t_uint		borders[combs->count];

	fill_borders(borders, combs);
	way = combs->count;
	cur_count = 0;
	while (way > 0)
		if ((__int64_t)ants - (way - 1) > borders[way - 1])
		{
			tmp = (ants - (way - 1) - borders[way - 1]) / way;
			tmp += (tmp) ? 0 : 1;
			cur_count += tmp;
			ants -= (way * tmp);
		}
		else
			combs->ways[--way].ants = cur_count;
	return ((combs->moves = combs->ways[0].ants + combs->ways[0].len - 1));
}
