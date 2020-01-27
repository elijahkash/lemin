/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   restore_ways.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Kashnitskiy <elijahkash.code@gmail.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/09 13:59:58 by mtrisha           #+#    #+#             */
/*   Updated: 2020/01/27 17:07:02 by Kashnitskiy      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <restore_ways.h>

static void	form_result(t_enum_ways *restrict res, t_vect *restrict way)
{
	t_uint	i;

	i = 0;
	res->nodes_mem = way->mem;
	while (i < res->count)
	{
		res->ways[i].nodes = vect(way, (size_t)res->ways[i].nodes);
		i++;
	}
}

static void	find_way(t_vect *restrict way, t_uint first_node,
						t_graph *restrict graph)
{
	t_iter				iter[1];
	t_connect *restrict	tmp;

	vect_add(way, &first_node);
	iter_init(iter, graph->nodes[first_node], ITER_FORBIDDEN);
	while (*(t_uint *)vect_top(way) != graph->end)
	{
		tmp = iter_next(iter);
		vect_add(way, ft_z(tmp->dst));
		iter_init(iter, graph->nodes[tmp->dst], ITER_FORBIDDEN);
	}
}

void		restore_ways(t_enum_ways *restrict res, t_graph *restrict graph,
							t_uint ways_count)
{
	t_uint				i;
	t_iter				iter[1];
	t_connect *restrict	tmp;
	t_vect				way;
	size_t				way_index;

	way_index = 0;
	vect_init(&way, sizeof(t_uint), 256);
	i = 0;
	enum_ways_init(res, ways_count);
	iter_init(iter, graph->nodes[graph->start], ITER_FORBIDDEN);
	while ((tmp = iter_next(iter)))
	{
		find_way(&way, tmp->dst, graph);
		way_init(res->ways + i, (t_uint *)way_index, way.curlen - way_index);
		way_index = way.curlen;
		i++;
	}
	vect_shrink(&way, 0);
	form_result(res, &way);
	ft_qsort(res->ways, res->count, sizeof(t_way), comp_way_by_len);
}
