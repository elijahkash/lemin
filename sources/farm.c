/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   farm.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtrisha <mtrisha@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/06 13:18:12 by mtrisha           #+#    #+#             */
/*   Updated: 2019/12/22 17:43:58 by mtrisha          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <farm.h>

#include <libft.h>
#include <input_errors.h>

/*
** =============================================================================
** =============================================================================
** =============================================================================
*/

inline void				node_mark(t_node *restrict node, t_int connect_state,
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

inline int				node_info_cmp(const void *a, const void *b)
{
	return (((t_node_info *)a)->weight - ((t_node_info *)b)->weight);
}

inline int				node_info_cmp_rev(const void *a, const void *b)
{
	return (((t_node_info *)b)->weight - ((t_node_info *)a)->weight);
}

/*
** =============================================================================
** =============================================================================
** =============================================================================
*/

inline void				full_connect_reverse(t_full_connect connect)
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

/*
** =============================================================================
** =============================================================================
** =============================================================================
*/

static int				count_node_connects(t_farm *restrict farm,
											t_uint *restrict node_connects)
{
	t_uint				same_connects;
	t_uint				i;
	t_dnbr *restrict	tmp;
	t_uint				len;

	same_connects = 0;
	i = 0;
	len = farm->connects.curlen - 1;
	while (i < len)
	{
		tmp = (t_dnbr *)vect(&(farm->connects), i);
		node_connects[tmp->a]++;
		node_connects[tmp->b]++;
		same_connects += ((ft_memcmp(tmp, tmp + 1, sizeof(t_dnbr))) ? 0 : 1);
		i++;
	}
	tmp = (t_dnbr *)vect(&(farm->connects), i);
	node_connects[tmp->a]++;
	node_connects[tmp->b]++;
	return (same_connects ? 1 : 0);
}

static void				graph_fill(t_graph *restrict graph,
									t_farm *restrict farm,
									t_uint *restrict node_connects)
{
	t_uint	i;
	void	*current_pos;
	t_dnbr	*tmp;

	i = 0;
	current_pos = graph->mem;
	while (i < graph->size)
	{
		graph->nodes[i] = current_pos;
		current_pos += sizeof(t_node) + sizeof(t_connect) * node_connects[i];
		i++;
	}
	i = 0;
	while (i < farm->connects.curlen)
	{
		tmp = vect(&(farm->connects), i);
		graph_add_connect(graph, tmp->a, tmp->b);
		graph_add_connect(graph, tmp->b, tmp->a);
		i++;
	}
}

int						graph_init(t_graph *restrict graph,
									t_farm *restrict farm)
{
	t_uint	node_connects[farm->names.curlen];

	graph->size = farm->names.curlen;
	ft_bzero(node_connects, sizeof(t_uint) * graph->size);
	vect_sort(&(farm->connects), dnbr_cmp, ft_qsort);
	if (count_node_connects(farm, node_connects))
		return (SAME_WAYS);
	graph->nodes = ft_malloc(sizeof(t_node *) * graph->size);
	graph->mem = ft_memalloc(sizeof(t_node) * graph->size +
							sizeof(t_connect) * farm->connects.curlen * 2);
	graph_fill(graph, farm, node_connects);
	graph->start = vect_bin_find(&(farm->names),
					ft_p(vect(&(farm->chars), farm->start)), ft_scmp) - 1;
	graph->end = vect_bin_find(&(farm->names),
					ft_p(vect(&(farm->chars), farm->end)), ft_scmp) - 1;
	return (0);
}

void					graph_del(t_graph *restrict graph)
{
	ft_free(graph->nodes);
	ft_free(graph->mem);
	graph->nodes = NULL;
	graph->mem = NULL;
	graph->size = 0;
}

inline t_node			*graph_node(t_graph *restrict graph, t_uint index)
{
	return (graph->nodes[index]);
}

inline t_connect		*graph_node_connects(t_node *restrict node)
{
	return ((t_connect *)(node + 1));
}

void					graph_add_connect(t_graph *restrict graph,
											t_uint src, t_uint dst)
{
	t_node		*restrict node;
	t_connect	*restrict con;

	node = graph->nodes[src];
	con = ((t_connect *)(node + 1)) + (node->count_connects)++;
	con->dst = dst;
	con->state = CONNECT_BASE_STATE;
}

static t_connect		*graph_connect_find(t_connect *restrict connects,
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

inline t_connect		*graph_connect(t_node *restrict src, t_uint dst)
{
	return (graph_connect_find(((t_connect *)(src + 1)),
								src->count_connects, dst));
}

void					graph_clear_state(t_graph *restrict graph)
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

t_connect				*(*g_iter_func[])(t_iter *restrict iter) = {
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

/*
** =============================================================================
** =============================================================================
** =============================================================================
*/

void					farm_init(t_farm *restrict farm)
{
	farm->ants = 0;
	farm->start = FARM_INIT_SE_VALUES;
	farm->end = FARM_INIT_SE_VALUES;
	farm->connects.mem = NULL;
	farm->chars.mem = NULL;
	farm->graph.mem = NULL;
}

void					farm_del(t_farm *restrict farm)
{
	if (farm->chars.mem)
		farm_del_rooms(farm);
	if (farm->connects.mem)
		farm_del_connects(farm);
	if (farm->graph.mem)
		graph_del(&(farm->graph));
}

void					farm_init_rooms(t_farm *restrict farm)
{
	vect_init(&(farm->chars), sizeof(char), FARM_INIT_CHARS_COUNT);
	vect_init(&(farm->names), sizeof(char *), FARM_INIT_ROOM_COUNT);
}

void					farm_init_connects(t_farm *restrict farm)
{
	vect_init(&(farm->connects), sizeof(t_dnbr),
				(size_t)(farm->names.curlen * FARM_INIT_CONNECTS_PER_ROOM));
}

void					farm_del_connects(t_farm *restrict farm)
{
	vect_del(&(farm->connects));
}

void					farm_del_rooms(t_farm *restrict farm)
{
	vect_del(&(farm->chars));
	vect_del(&(farm->names));
}

/*
** =============================================================================
** =============================================================================
** =============================================================================
*/

inline int				dnbr_cmp(const void *number_1,
									const void *number_2)
{
	int	res;

	res = ((t_dnbr *)number_1)->a - ((t_dnbr *)number_2)->a;
	return (res ? res : (((t_dnbr *)number_1)->b - ((t_dnbr *)number_2)->b));
}

/*
** =============================================================================
** =============================================================================
** =============================================================================
*/

inline void				way_init(t_way *restrict way, t_uint *restrict arr,
									t_uint len)
{
	way->ants = 0;
	way->len = len;
	way->ants_names = NULL;
	way->nodes = arr;
}

inline void				way_del(t_way way)
{
	ft_free(way.ants_names);
}

inline int				comp_way_by_len(const void *restrict way1,
										const void *restrict way2)
{
	return (((t_way *)way1)->len - ((t_way *)way2)->len);
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
	combs->nodes_mem = NULL;
	combs->ways = (t_way *)ft_malloc(sizeof(t_way *) * count);
}

void					enum_ways_del(t_enum_ways *restrict combs)
{
	if (combs->count && combs->ways[0].ants_names)
	{
		while (combs->count)
			way_del(combs->ways[--(combs->count)]);
	}
	if (combs->nodes_mem)
		ft_free(combs->nodes_mem);
	if (combs->ways)
		ft_free(combs->ways);
	combs->moves = 0;
	combs->nodes_mem = NULL;
	combs->ways = NULL;
}

static void				fill_borders(t_uint *restrict borders,
										t_enum_ways *restrict combs)
{
	t_uint	i;
	t_uint	j;
	t_uint	len;

	len = combs->count;
	i = 0;
	while (i < len)
	{
		borders[i] = 0;
		j = 0;
		while (j < i)
		{
			borders[i] += (combs->ways[i].len - combs->ways[j].len);
			j++;
		}
		i++;
	}
}

t_uint					count_moves(t_enum_ways *restrict combs, t_uint ants)
{
	t_uint		cur_count;
	t_uint		tmp;
	t_uint		way;
	t_uint		borders[combs->count];

	fill_borders(borders, combs);
	way = combs->count;
	cur_count = 0;
	while (way > 0)
		if ((__int64_t)ants - (way - 1) > borders[way - 1])
		{
			tmp = (ants - (way - 1) - borders[way - 1]) / way;
			tmp += (tmp) ? 0 : 1;
			cur_count += tmp;
			ants -= (way * tmp);
		}
		else
			combs->ways[--way].ants = cur_count;
	return ((combs->moves = combs->ways[0].ants + combs->ways[0].len - 1));
}
