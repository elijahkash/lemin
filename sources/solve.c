/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   solve.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtrisha <mtrisha@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/09 16:17:25 by mtrisha           #+#    #+#             */
/*   Updated: 2019/12/10 18:11:16 by mtrisha          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <solve.h>

#include <farm.h>
#include <libft.h>

static void		reverse_new_way(t_graph *restrict graph)
{
	t_uint				cur_iter;
	t_uint				cur_bfs_level;
	t_connect *restrict	src_to_dst;
	t_connect *restrict	dst_to_src;
	t_iter				iter;
	t_full_connect		connect;
	t_node *restrict	ptr;

	cur_iter = graph->end;
	cur_bfs_level = graph_node(graph, graph->end)->bfs_level;
	while (cur_bfs_level-- > 0)
	{
		iter_init(&iter, graph->nodes[cur_iter], ITER_ALL);
		while ((src_to_dst = iter_next(&iter)))
		{
			ptr = graph_node(graph, src_to_dst->dst);
			dst_to_src = graph_connect(graph, src_to_dst->dst, cur_iter);
			if (ptr->bfs_level == cur_bfs_level &&
				(dst_to_src->state != CONNECT_FORBIDDEN) &&
				(ptr->separate == 0 || ptr->marked_out ||
				dst_to_src->state == CONNECT_NEGATIVE))
				break ;
		}
		connect.dst_to_src = dst_to_src;
		connect.src_to_dst = src_to_dst;
		cur_iter = connect.src_to_dst->dst;
		full_connect_reverse(connect);
		node_reverse(graph_node(graph, cur_iter));
	}
	node_reverse(graph_node(graph, graph->start));
}

void			add_nodes(t_uint item, t_graph *restrict graph,
							t_deq *restrict marked)
{
	t_iter					iter;
	t_connect *restrict		connect;
	t_node *restrict		node;
	t_uint					bfs_level;

	bfs_level = graph_node(graph, item)->bfs_level + 1;
	iter_init(&iter, graph->nodes[item], ITER_BY_NODE);
	while ((connect = iter_next(&iter)))
	{
		node = graph_node(graph, connect->dst);
		if (node->marked == 0)
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

	deq_init(&marked, sizeof(t_uint), 128);
	deq_push_back(&marked, &(graph->start));
	graph_node(graph, graph->start)->marked = 1; //TODO: optimise
	graph_node(graph, graph->start)->bfs_level = 0;
	while (marked.curlen)
	{
		add_nodes(*(t_uint *)deq_pop_front(&marked), graph, &marked);
		if (graph_node(graph, graph->end)->marked)
			break ;
	}
	deq_del(&marked);
	if (graph_node(graph, graph->end)->marked)
		reverse_new_way(graph);
	res = (graph_node(graph, graph->end)->marked) ? 1 : 0;
	graph_clear_state(graph);
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
		res->ways[i].len = way.curlen;
		//TODO: init
		res->ways[i].nodes = ft_malloc(sizeof(t_uint) * way.curlen);
		ft_memcpy(res->ways[i].nodes, way.mem, sizeof(t_uint) * way.curlen);
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
