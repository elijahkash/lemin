/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   solve.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtrisha <mtrisha@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/09 16:17:25 by mtrisha           #+#    #+#             */
/*   Updated: 2019/12/30 16:08:40 by mtrisha          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <solve.h>

#include <farm.h>
#include <libft.h>

void			insert_in_queue(t_alst *restrict marked, t_node_info data);
t_node_info		prepare_data(t_node_info src, t_full_connect item);
void			change_weight(t_alst *marked, t_uint item, t_int weight);
void			check_node(t_alst *restrict marked, t_node_info src,
							t_full_connect item);

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

inline void		insert_in_queue(t_alst *restrict marked, t_node_info data)
{
	size_t cur;
	size_t next;

	cur = marked->head;
	if (marked->curlen == 0 ||
	(((t_node_info *)alst(marked, cur))->weight >= data.weight))
		alst_add_head(marked, &data);
	else if (((t_node_info *)alst(marked, marked->tail))->weight <=
													data.weight)
		alst_add_tail(marked, &data);
	else
	{
		next = marked->list[marked->head].next;
		while (((t_node_info *)alst(marked, next))->weight < data.weight)
		{
			cur = next;
			next = marked->list[next].next;
		}
		alst_add_after(marked, cur, &data);
	}
	data.ptr->in_queue = 1;
}

inline t_node_info		prepare_data(t_node_info src, t_full_connect item)
{
	t_node_info	res;

	res.ptr = item.dst;
	res.self = item.src_dst->dst;
	res.weight = src.weight + item.src_dst->state;
	return (res);
}

inline void			change_weight(t_alst *restrict marked, t_uint item,
															t_int weight)
{
	t_node_info *tmp;
	size_t		cur;
	size_t		next;

	cur = marked->head;
	next = marked->list[cur].next;
	if ((tmp = ((t_node_info *)alst(marked, cur)))->self == item)
	{
		tmp->weight = weight;
		return ;
	}
	while ((tmp = ((t_node_info *)alst(marked, next)))->self != item)
	{
		cur = next;
		next = marked->list[cur].next;
	}
	tmp = ((t_node_info *)alst_pop_after(marked, cur));
	tmp->weight = weight;
	insert_in_queue(marked, *tmp);
}

inline void			check_node(t_alst *restrict marked, t_node_info src,
							t_full_connect item)
{
	t_node_info		tmp;

	if (item.dst->marked == 0)
	{
		insert_in_queue(marked, (tmp = prepare_data(src, item)));
		node_mark(item.dst, item.src_dst->state, tmp.weight, src.self);
	}
	else if (item.dst->separate && item.dst->marked_sep == MARKED_IN &&
			item.src_dst->state == CONNECT_NEGATIVE)
	{
		tmp = prepare_data(src, item);
		item.dst->weight <= tmp.weight ? (item.dst->marked_sep = MARKED_OUT) :
			node_mark(item.dst, item.src_dst->state, tmp.weight, src.self);
		(item.dst->in_queue == 0) ? insert_in_queue(marked, tmp) :
			change_weight(marked, item.src_dst->dst, tmp.weight);
	}
	else if (item.dst->separate == 0 && item.src->separate &&
			item.dst->weight > (tmp.weight = src.weight + item.src_dst->state))
	{
		tmp.ptr = item.dst;
		tmp.self = item.src_dst->dst;
		node_mark(item.dst, item.src_dst->state, tmp.weight, src.self);
		(item.dst->in_queue == 0) ? insert_in_queue(marked, tmp) :
			change_weight(marked, item.src_dst->dst, tmp.weight);
	}
}

void			add_nodes(t_graph *restrict graph, t_alst *restrict marked)
{
	t_iter					iter[1];
	t_node_info				src;
	t_full_connect			item;

	src = *(t_node_info *)alst_pop_head(marked);
	item.src = src.ptr;
	item.src->in_queue = 0;
	iter_init(iter, item.src, ITER_BY_NODE);
	while ((item.src_dst = iter_next(iter)))
	{
		item.dst = graph_node(graph, item.src_dst->dst);
		check_node(marked, src, item);
	}
}

static int		find_new_way(t_graph *restrict graph)
{
	t_alst		marked;
	t_node		*end_node;
	t_node_info	tmp;

	tmp.self = graph->start;
	tmp.weight = 0;
	tmp.ptr = graph_node(graph, graph->start);
	alst_init(&marked, sizeof(t_node_info), 256);
	alst_add_head(&marked, &tmp);
	node_mark(tmp.ptr, 0, 0, 0);
	end_node = graph_node(graph, graph->end);
	while (end_node->marked == 0 && marked.curlen)
		add_nodes(graph, &marked);
	alst_del(&marked);
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
						t_uint ants)
{
	t_uint		min_moves;
	t_enum_ways	tmp;
	t_uint		max_count;

	max_count = graph_node(graph, graph->end)->count_connects;
	tmp.count = 0;
	min_moves = __UINT32_MAX__;
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
