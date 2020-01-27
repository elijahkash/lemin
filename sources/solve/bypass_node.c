/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bypass_node.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Kashnitskiy <elijahkash.code@gmail.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/09 14:06:34 by mtrisha           #+#    #+#             */
/*   Updated: 2020/01/27 16:41:21 by Kashnitskiy      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <bypass_node.h>

static void			insert_in_queue(t_alst *restrict marked, t_node_info data)
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

static t_node_info	prepare_data(t_node_info src, t_full_connect item)
{
	t_node_info	res;

	res.ptr = item.dst;
	res.self = item.src_dst->dst;
	res.weight = src.weight + item.src_dst->state;
	return (res);
}

static void			change_weight(t_alst *restrict marked, t_uint item,
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

static void			check_node(t_alst *restrict marked, t_node_info src,
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
	else if (item.dst->weight > (tmp.weight = src.weight + item.src_dst->state))
	{
		tmp.ptr = item.dst;
		tmp.self = item.src_dst->dst;
		node_mark(item.dst, item.src_dst->state, tmp.weight, src.self);
		(item.dst->in_queue == 0) ? insert_in_queue(marked, tmp) :
			change_weight(marked, item.src_dst->dst, tmp.weight);
	}
}

void				bypass_node(t_graph *restrict graph,
								t_alst *restrict marked)
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
