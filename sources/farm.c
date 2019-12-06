/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   farm.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtrisha <mtrisha@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/06 13:18:12 by mtrisha           #+#    #+#             */
/*   Updated: 2019/12/06 15:42:34 by mtrisha          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <farm.h>

#include <libft.h>

/*
** =============================================================================
** =============================================================================
** =============================================================================
*/

//TODO: need marked???
void					node_reverse(t_node *restrict node)
{
	t_iter	iter;

	if (node->separate)
	{
		iter_init(&iter, node, ITER_NEGATIVE);
		if (iter_next(&iter) == NULL)
		{
			node->separate = 0;
			// node->marked = 1;
			// node->marked_in = 0;
			// node->marked_out = 0;
		}
	}
	else
	{
		//TODO: check on start???
		// node->marked_out = 1;
		node->separate = 1;
	}
}

/*
** =============================================================================
** =============================================================================
** =============================================================================
*/

void					full_connect_reverse(t_full_connect connect)
{
	if (connect.src_to_dst->state & (CONNECT_FORBIDDEN || CONNECT_NEGATIVE))
	{
		connect.src_to_dst->state = CONNECT_BASE_STATE;
		connect.dst_to_src->state = CONNECT_BASE_STATE;
	}
	else
	{
		connect.src_to_dst->state = CONNECT_NEGATIVE;
		connect.dst_to_src->state = CONNECT_FORBIDDEN;
	}
}

/*
** =============================================================================
** =============================================================================
** =============================================================================
*/

static inline t_connect	*iter_next_all(t_iter *restrict iter)
{
	return ((iter->i != iter->count_connects) ?
			iter->connects + (iter->i)++ : NULL);
}

static t_connect		*iter_next_allowed(t_iter *restrict iter)
{
	t_connect	*tmp;

	while ((tmp = iter_next_all(iter)) && (tmp->state & CONNECT_FORBIDDEN))
		continue ;
	return (tmp);
}

static t_connect		*iter_next_negative(t_iter *restrict iter)
{
	t_connect	*tmp;

	while ((tmp = iter_next_all(iter)) && !(tmp->state & CONNECT_NEGATIVE))
		continue ;
	return (tmp);
}

static t_connect		*iter_next_forbidden(t_iter *restrict iter)
{
	t_connect	*tmp;

	while ((tmp = iter_next_all(iter)) && !(tmp->state & CONNECT_FORBIDDEN))
		continue ;
	return (tmp);
}

t_connect		*(*iter_func[])(t_iter *restrict iter) = {
	iter_next_all,
	iter_next_allowed,
	iter_next_negative,
	iter_next_forbidden
};

void					iter_init(t_iter *restrict iter, t_node *restrict node,
								t_uint type)
{
	iter->count_connects = node->count_connects;
	iter->i = 0;
	iter->connects = graph_node_connects(node);
	if (type == ITER_BY_NODE)
		iter->func = node->marked_in ? ITER_NEGATIVE : ITER_ALLOWED;
	else
		iter->func = type;
}

inline t_connect		*iter_next(t_iter *restrict iter)
{
	return (iter_func[iter->func](iter));
}

/*
** =============================================================================
** =============================================================================
** =============================================================================
*/

void					way_init(t_way *restrict way, t_uint *restrict arr,
								t_uint len)
{
	way->ants = 0;
	way->len = len;
	way->nodes = ft_memcpy(ft_malloc(sizeof(t_uint) * len), arr, len);
}

inline void				way_del(t_way way)
{
	ft_free(way.nodes);
}

/*
** =============================================================================
** =============================================================================
** =============================================================================
*/

void					enum_ways_init(t_enum_ways *restrict combs,
										t_uint count)
{
	combs->moves = 0;
	combs->count = count;
	combs->ways = (t_way *)ft_malloc(sizeof(t_way *) * count);
}

void					enum_ways_del(t_enum_ways *restrict combs)
{
	while (combs->count)
		way_del(combs->ways[--(combs->count)]);
	combs->moves = 0;
	ft_free(combs->ways);
	combs->ways = NULL;
}

long long				count_moves(t_enum_ways *restrict combs, long long ants)
{
	long long	k = 0;
	t_uint		way;
	t_uint		border[combs->count];

	way = combs->count;
	if (way == 1)
	{
		combs->ways[0].ants = ants;
		combs->moves = ants + combs->ways[0].len - 1;
		return (combs->moves);
	}
	for(t_uint i = 0; i < way; i++)
	{
		border[i] = 0;
		for (t_uint j = 0; j < i; j++)
			border[i] += (combs->ways[i].len - combs->ways[j].len);
	}
	while (way > 0)
	{
		if (ants > border[way - 1])
		{
			k++;
			ants -= way;
		}
		else
			combs->ways[--way].ants = k;
	}
	combs->moves = combs->ways[0].ants + combs->ways[0].len - 1;
	return (combs->moves);
}
