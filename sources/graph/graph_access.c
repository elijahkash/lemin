/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   graph_access.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Kashnitskiy <elijahkash.code@gmail.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/09 15:37:18 by mtrisha           #+#    #+#             */
/*   Updated: 2020/01/27 17:07:12 by Kashnitskiy      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <graph.h>

inline t_node		*graph_node(t_graph *restrict graph, t_uint index)
{
	return (graph->nodes[index]);
}

inline t_connect	*graph_node_connects(t_node *restrict node)
{
	return ((t_connect *)(node + 1));
}

static t_connect	*graph_connect_find(t_connect *restrict connects,
										t_uint count_connects, t_uint dst)
{
	t_uint			bot;
	t_uint			top;
	long long		tmp;

	if (!count_connects)
		return (NULL);
	top = count_connects - 1;
	bot = 0;
	while (bot != top)
	{
		tmp = (long long)dst - (long long)(connects[bot + (top - bot) / 2].dst);
		if (!tmp)
			return (connects + bot + (top - bot) / 2);
		if (tmp < 0)
			top = bot + (top - bot) / 2;
		else
			bot += (top - bot) / 2 + 1;
	}
	tmp = (long long)dst - (long long)(connects[bot].dst);
	return (tmp ? NULL : connects + bot);
}

inline t_connect	*graph_connect(t_node *restrict src, t_uint dst)
{
	return (graph_connect_find(((t_connect *)(src + 1)),
								src->count_connects, dst));
}
