/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   solve.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtrisha <mtrisha@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/09 16:17:25 by mtrisha           #+#    #+#             */
/*   Updated: 2019/12/17 17:48:27 by mtrisha          ###   ########.fr       */
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
	t_full_connect		itm;

	cur_iter = graph->end;
	itm.src = graph_node(graph, cur_iter);
	cur_bfs_level = itm.src->bfs_level;
	while (cur_bfs_level-- > 0)
	{
		iter_init(&iter, itm.src, ITER_ALL);
		while ((itm.src_to_dst = iter_next(&iter)))
		{
			itm.dst = graph_node(graph, itm.src_to_dst->dst);
			itm.dst_to_src = graph_connect(itm.dst, cur_iter);
			if (itm.dst->bfs_level == cur_bfs_level && itm.dst_to_src->state
			!= CONNECT_FORBIDDEN && (itm.dst->separate == 0 ||
			itm.dst->marked_out || itm.dst_to_src->state == CONNECT_NEGATIVE))
				break ;
		}
		cur_iter = itm.src_to_dst->dst;
		itm.src = itm.dst;
		full_connect_reverse(itm);
	}
	itm.dst->separate = 0;
}

void			add_nodes(t_graph *restrict graph, t_deq *restrict marked,
													t_deq *restrict remark)
{
	t_iter					iter;
	t_connect *restrict		connect;
	t_node *restrict		node;
	t_uint					bfs_level;
	t_dnbr					tmp;

	node = graph_node(graph, *(t_uint *)deq_pop_front(marked));
	bfs_level = node->bfs_level + 1;
	iter_init(&iter, node, ITER_BY_NODE);
	while ((connect = iter_next(&iter)))
	{
		if ((node = graph_node(graph, connect->dst))->marked == 0)
		{
			deq_push_back(marked, ft_z(connect->dst));
			node_mark(node, connect->state, bfs_level);
		}
		else if (node->marked_in && connect->state == CONNECT_NEGATIVE)
		{
			tmp.a = connect->dst;
			tmp.b = bfs_level;
			deq_push_back(remark, &tmp);
		}
	}
}

static int		find_new_way(t_graph *restrict graph)
{
	t_deq	marked;
	t_deq	remark;
	t_node	*end_node;
	t_dnbr	tmp;

	graph_clear_state(graph);
	deq_init(&marked, sizeof(t_uint), 256);
	deq_init(&remark, sizeof(t_dnbr), 128);
	deq_push_back(&marked, &(graph->start));
	node_mark(graph_node(graph, graph->start), 0, 0);
	end_node = graph_node(graph, graph->end);
	while (TRUE)
	{
		while (end_node->marked == 0 && marked.curlen)
			add_nodes(graph, &marked, &remark);
		if (end_node->marked || remark.curlen == 0)
			break ;
		tmp = *((t_dnbr *)deq_pop_front(&remark));
		deq_push_front(&marked, &(tmp.a));
		node_mark(graph_node(graph, tmp.a), CONNECT_NEGATIVE, tmp.b);
	}
	deq_del(&marked);
	deq_del(&remark);
	(end_node->marked) ? reverse_new_way(graph) : 0;
	return (end_node->marked);
}

static void		form_res(t_enum_ways *restrict res, t_vect *restrict way)
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
	size_t				way_index;

	way_index = 0;
	vect_init(&way, sizeof(t_uint), 256);
	i = 0;
	res->ways = ft_malloc(sizeof(t_way) * res->count);
	iter_init(iter, graph->nodes[graph->start], ITER_FORBIDDEN);
	while ((tmp = iter_next(iter)))
	{
		find_way(&way, tmp->dst, graph);
		way_init(res->ways + i, (t_uint *)way_index, way.curlen - way_index);
		way_index = way.curlen;
		i++;
	}
	vect_shrink(&way, 0);
	form_res(res, &way);
	ft_qsort(res->ways, res->count, sizeof(t_way), comp_way_by_len);
}

int				solve(t_enum_ways *restrict result, t_graph *restrict graph,
						long long ants)
{
	t_uint		k;
	long long	min_moves;
	t_enum_ways	tmp;

	k = 0;
	min_moves = __INT64_MAX__;
	while (k < graph_node(graph, graph->start)->count_connects &&
			k < graph_node(graph, graph->end)->count_connects &&
			find_new_way(graph))
	{
		tmp.count = ++k;
		find_ways(&tmp, graph);
		count_moves(&tmp, ants);
		if (tmp.moves <= min_moves)
		{
			min_moves = tmp.moves;
			enum_ways_del(result);
			*result = tmp;
		}
		else
			enum_ways_del(&tmp);
		if (tmp.moves > min_moves)
			break ;
	}
	return (0);
}
