/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   reverse_new_way.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtrisha <mtrisha@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/09 14:21:44 by mtrisha           #+#    #+#             */
/*   Updated: 2020/01/09 14:22:21 by mtrisha          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <reverse_new_way.h>

static void		find_new_parent(t_node *restrict node, t_graph *restrict graph)
{
	t_int				min_weight;
	t_connect *restrict connect;
	t_int				new_wt;
	t_iter				iter[1];
	t_uint				res;

	min_weight = __INT32_MAX__;
	res = node->parent;
	iter_init(iter, node, ITER_POSITIVE);
	while ((connect = iter_next(iter)))
		if ((new_wt = graph_node(graph, connect->dst)->weight) < min_weight)
		{
			min_weight = new_wt;
			res = connect->dst;
		}
	node->parent = res;
}

void			reverse_new_way(t_graph *restrict graph)
{
	t_uint				cur_item;
	t_full_connect		item;
	t_iter				iter[1];

	cur_item = graph->end;
	item.src = graph_node(graph, cur_item);
	while (cur_item != graph->start)
	{
		item.dst = graph_node(graph, item.src->parent);
		item.src_dst = graph_connect(item.src, item.src->parent);
		item.dst_src = graph_connect(item.dst, cur_item);
		(item.dst->separate && item.dst->marked_sep == MARKED_OUT &&
			item.src_dst->state == CONNECT_BASE_STATE) ? (item.dst->parent =
			iter_next(iter_init(iter, item.dst, ITER_FORBIDDEN))->dst) : 0;
		(item.dst->in_new_way) ? find_new_parent(item.dst, graph) : 0;
		item.dst->in_new_way = 1;
		full_connect_reverse(item);
		cur_item = item.src->parent;
		item.src = item.dst;
	}
	item.src->separate = 0;
}
