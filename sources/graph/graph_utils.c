/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   graph_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Kashnitskiy <elijahkash.code@gmail.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/09 15:39:41 by mtrisha           #+#    #+#             */
/*   Updated: 2020/01/27 17:07:11 by Kashnitskiy      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <graph.h>

inline void		node_mark(t_node *restrict node, t_int connect_state,
							t_int weight, t_uint parent)
{
	node->weight = weight;
	node->marked = 1;
	node->parent = parent;
	if (node->separate)
	{
		if (connect_state == CONNECT_NEGATIVE)
			node->marked_sep = MARKED_OUT;
		else
			node->marked_sep = MARKED_IN;
	}
}

inline void		full_connect_reverse(t_full_connect connect)
{
	if (connect.src_dst->state == CONNECT_FORBIDDEN ||
		connect.src_dst->state == CONNECT_NEGATIVE)
	{
		connect.src_dst->state = CONNECT_BASE_STATE;
		connect.dst_src->state = CONNECT_BASE_STATE;
		connect.dst->separate = 0;
	}
	else
	{
		connect.src_dst->state = CONNECT_NEGATIVE;
		connect.dst_src->state = CONNECT_FORBIDDEN;
		connect.src->separate = 1;
		connect.dst->separate = 1;
	}
}

void			graph_add_connect(t_graph *restrict graph, t_uint src,
									t_uint dst)
{
	t_node		*restrict node;
	t_connect	*restrict con;

	node = graph->nodes[src];
	con = ((t_connect *)(node + 1)) + (node->count_connects)++;
	con->dst = dst;
	con->state = CONNECT_BASE_STATE;
}

void			graph_clear_state(t_graph *restrict graph)
{
	t_uint	i;

	i = 0;
	while (i < graph->size)
	{
		graph->nodes[i]->marked = 0;
		graph->nodes[i]->weight = 0;
		graph->nodes[i]->parent = 0;
		graph->nodes[i]->in_queue = 0;
		graph->nodes[i]->in_new_way = 0;
		i++;
	}
	graph->nodes[graph->start]->separate = 0;
	graph->nodes[graph->end]->separate = 0;
}

void			graph_del(t_graph *restrict graph)
{
	ft_free(graph->nodes);
	ft_free(graph->mem);
	graph->nodes = NULL;
	graph->mem = NULL;
	graph->size = 0;
}
