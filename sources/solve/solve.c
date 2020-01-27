/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   solve.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Kashnitskiy <elijahkash.code@gmail.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/09 16:17:25 by mtrisha           #+#    #+#             */
/*   Updated: 2020/01/27 17:07:04 by Kashnitskiy      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <solve.h>

#include <bypass_node.h>
#include <reverse_new_way.h>
#include <restore_ways.h>

static int		find_new_way(t_graph *restrict graph)
{
	t_alst		bypass_queue;
	t_node		*end_node;
	t_node_info	tmp;

	tmp.self = graph->start;
	tmp.weight = 0;
	tmp.ptr = graph_node(graph, graph->start);
	alst_init(&bypass_queue, sizeof(t_node_info), 256);
	alst_add_head(&bypass_queue, &tmp);
	node_mark(tmp.ptr, 0, 0, 0);
	end_node = graph_node(graph, graph->end);
	while (end_node->marked == 0 && bypass_queue.curlen)
		bypass_node(graph, &bypass_queue);
	alst_del(&bypass_queue);
	(end_node->marked) ? reverse_new_way(graph) : 0;
	return (end_node->marked);
}

void			solve(t_enum_ways *restrict result, t_graph *restrict graph,
						t_uint ants)
{
	t_uint		min_moves;
	t_enum_ways	tmp;
	t_uint		max_ways_count;
	t_uint		cur_ways_count;

	max_ways_count = graph_node(graph, graph->end)->count_connects;
	cur_ways_count = 0;
	min_moves = __UINT32_MAX__;
	while (find_new_way(graph))
	{
		cur_ways_count++;
		restore_ways(&tmp, graph, cur_ways_count);
		count_moves(&tmp, ants);
		if (tmp.moves <= min_moves)
		{
			min_moves = tmp.moves;
			enum_ways_del(result);
			*result = tmp;
		}
		else
			enum_ways_del(&tmp);
		if ((tmp.moves > min_moves) || (cur_ways_count == max_ways_count))
			break ;
		graph_clear_state(graph);
	}
}
