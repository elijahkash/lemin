/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   graph_iter.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtrisha <mtrisha@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/06 13:18:12 by mtrisha           #+#    #+#             */
/*   Updated: 2020/01/09 15:47:33 by mtrisha          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <graph.h>

static inline t_connect	*iter_next_all(t_iter *restrict iter)
{
	return ((iter->i != iter->count_connects) ?
			iter->connects + (iter->i)++ : NULL);
}

static t_connect		*iter_next_allowed(t_iter *restrict iter)
{
	t_connect	*restrict tmp;

	while ((tmp = iter_next_all(iter)) && (tmp->state == CONNECT_FORBIDDEN))
		continue ;
	return (tmp);
}

static t_connect		*iter_next_negative(t_iter *restrict iter)
{
	t_connect	*restrict tmp;

	while ((tmp = iter_next_all(iter)) && !(tmp->state == CONNECT_NEGATIVE))
		continue ;
	return (tmp);
}

static t_connect		*iter_next_forbidden(t_iter *restrict iter)
{
	t_connect	*restrict tmp;

	while ((tmp = iter_next_all(iter)) && !(tmp->state == CONNECT_FORBIDDEN))
		continue ;
	return (tmp);
}

static t_connect		*iter_next_positive(t_iter *restrict iter)
{
	t_connect	*restrict tmp;

	while ((tmp = iter_next_all(iter)) && (tmp->state != CONNECT_BASE_STATE))
		continue ;
	return (tmp);
}

static t_connect		*(*g_iter_func[])(t_iter *restrict iter) = {
	iter_next_all,
	iter_next_allowed,
	iter_next_negative,
	iter_next_forbidden,
	iter_next_positive
};

t_iter					*iter_init(t_iter *restrict iter, t_node *restrict node,
								t_uint type)
{
	iter->count_connects = node->count_connects;
	iter->i = 0;
	iter->connects = graph_node_connects(node);
	if (type == ITER_BY_NODE)
		iter->func = (node->separate && node->marked_sep == MARKED_IN) ?
						ITER_NEGATIVE : ITER_ALLOWED;
	else
		iter->func = type;
	return (iter);
}

inline t_connect		*iter_next(t_iter *restrict iter)
{
	return (g_iter_func[iter->func](iter));
}
