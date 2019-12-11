/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   solve.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtrisha <mtrisha@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/09 16:17:25 by mtrisha           #+#    #+#             */
/*   Updated: 2019/12/11 17:38:26 by mtrisha          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <solve.h>

#include <farm.h>
#include <libft.h>

static void		reverse_new_way(t_graph *restrict graph)
{
	t_uint				cur_iter;
	t_uint				cur_bfs_level;
	t_iter				iter;
	t_full_connect		connect;
	t_node *restrict	node;

	node = graph_node(graph, (cur_iter = graph->end));
	cur_bfs_level = node->bfs_level;
	while (cur_bfs_level-- > 0)
	{
		iter_init(&iter, node, ITER_ALL);
		while ((connect.src_to_dst = iter_next(&iter)))
		{
			node = graph_node(graph, connect.src_to_dst->dst);
			connect.dst_to_src = graph_connect(node, cur_iter);
			if ((node->bfs_level == cur_bfs_level) &&
				(connect.dst_to_src->state != CONNECT_FORBIDDEN) &&
				(node->separate == 0 || node->marked_out ||
				connect.dst_to_src->state == CONNECT_NEGATIVE))
				break ;
		}
		cur_iter = connect.src_to_dst->dst;
		full_connect_reverse(connect);
		node_reverse(node);
	}
	node_reverse(node);
}

void			add_nodes(t_uint item, t_graph *restrict graph,
							t_deq *restrict marked)
{
	t_iter					iter;
	t_connect *restrict		connect;
	t_node *restrict		node;
	t_uint					bfs_level;

	node = graph_node(graph, item);
	bfs_level = node->bfs_level + 1;
	iter_init(&iter, node, ITER_BY_NODE);
	while ((connect = iter_next(&iter)))
	{
		if ((node = graph_node(graph, connect->dst))->marked == 0)
		{
			deq_push_back(marked, ft_z(connect->dst));
			mark_node(node, connect, bfs_level);
		}
	}
}

static int		find_new_way(t_graph *restrict graph)
{
	t_deq	marked;
	int		res;
	t_node	*end_node;

	graph_clear_state(graph);
	deq_init(&marked, sizeof(t_uint), 128);
	deq_push_back(&marked, &(graph->start));
	end_node = graph_node(graph, graph->start);
	end_node->marked = 1;
	end_node->bfs_level = 0;
	end_node = graph_node(graph, graph->end);
	while (marked.curlen)
	{
		add_nodes(*(t_uint *)deq_pop_front(&marked), graph, &marked);
		if (end_node->marked)
			break ;
	}
	deq_del(&marked);
	if ((res = end_node->marked))
		reverse_new_way(graph);
	return (res);
}

static void		find_way(t_vect *restrict way, t_uint first_node,
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

static void		find_ways(t_enum_ways *restrict res, t_graph *restrict graph)
{
	t_uint				i;
	t_iter				iter[1];
	t_connect *restrict	tmp;
	t_vect				way;

	vect_init(&way, sizeof(t_uint), 256);
	i = 0;
	res->ways = ft_malloc(sizeof(t_way) * res->count);
	iter_init(iter, graph->nodes[graph->start], ITER_FORBIDDEN);
	while ((tmp = iter_next(iter)))
	{
		way.curlen = 0;
		find_way(&way, tmp->dst, graph);
		way_init(res->ways + i, way.mem, way.curlen);
		i++;
	}
	ft_qsort(res->ways, res->count, sizeof(t_way), comp_way_by_len);
}

int				solve(t_enum_ways *restrict result, t_graph *restrict graph,
						long long ants)
{
	t_uint		k;
	long long	min_moves;
	t_enum_ways	tmp;

	k = 0;
	min_moves = 0;
	while (find_new_way(graph))
	{
		k++;
		tmp.count = k;
		find_ways(&tmp, graph);
		count_moves(&tmp, ants);
		if (k == 1 || tmp.moves < min_moves)
		{
			min_moves = tmp.moves;
			enum_ways_del(result);
			*result = tmp;
		}
		else
			enum_ways_del(&tmp);
	}
	return (0);
}
