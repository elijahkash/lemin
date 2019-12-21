/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   solve.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtrisha <mtrisha@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/09 16:17:25 by mtrisha           #+#    #+#             */
/*   Updated: 2019/12/21 22:12:22 by mtrisha          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <solve.h>

#include <farm.h>
#include <libft.h>

static void		reverse_new_way(t_graph *restrict graph)
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
		if (item.dst->separate && item.dst->marked_sep == MARKED_OUT &&
			item.src_dst->state == CONNECT_BASE_STATE)
		{
			iter_init(iter, item.dst, ITER_FORBIDDEN);
			item.dst->parent = iter_next(iter)->dst;
		}
		full_connect_reverse(item);
		cur_item = item.src->parent;
		item.src = item.dst;
	}
	item.src->separate = 0;
}

void			add_nodes(t_graph *restrict graph, t_vect *restrict marked)
{
	t_iter					iter[1];
	t_node_info				src;
	t_full_connect			item;
	t_node_info				tmp;
	t_uint					k = 0;

	src = *(t_node_info *)vect_pop(marked);
	item.src = graph_node(graph, src.self);
	item.src->in_deq = 0;
	iter_init(iter, item.src, ITER_BY_NODE);
	while ((item.src_dst = iter_next(iter)))
	{
		item.dst = graph_node(graph, item.src_dst->dst);
		tmp.weight = src.weight + item.src_dst->state;
		tmp.self = item.src_dst->dst;
		if (item.dst->marked == 0)
		{
			vect_add(marked, &tmp);
			item.dst->in_deq = 1;
			k++;
			node_mark(item.dst, item.src_dst->state, tmp.weight, src.self);
		}
		else if (item.dst->separate && item.dst->marked_sep == MARKED_IN &&
				item.src_dst->state == CONNECT_NEGATIVE)
		{
			item.dst->weight <= tmp.weight ? (item.dst->marked_sep = MARKED_OUT)
			: node_mark(item.dst, item.src_dst->state, tmp.weight, src.self);
			if (item.dst->in_deq == 0)
				vect_add(marked, &tmp);
			item.dst->in_deq = 1;
			k++;
		}
	}
	//TODO: я сортирую первичные веса. а если они изменятся в условии выше?
	if (k)
		vect_sort(marked, node_info_cmp_rev, ft_qsort);
}

static int		find_new_way(t_graph *restrict graph)
{
	t_vect	marked;
	t_node	*end_node;

	vect_init(&marked, sizeof(t_node_info), 256);
	vect_add(&marked, &(graph->start));
	node_mark(graph_node(graph, graph->start), 0, 0, 0);
	end_node = graph_node(graph, graph->end);
	while (end_node->marked == 0 && marked.curlen)
		add_nodes(graph, &marked);
	vect_del(&marked);
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
	long long	min_moves;
	t_enum_ways	tmp;
	t_uint		max_count;

	max_count = graph_node(graph, graph->end)->count_connects;
	tmp.count = 0;
	min_moves = __INT64_MAX__;
	while (find_new_way(graph))
	{
		tmp.count++;
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
		if ((tmp.moves > min_moves) || (tmp.count == max_count))
			break ;
		graph_clear_state(graph);
	}
	return (0);
}
