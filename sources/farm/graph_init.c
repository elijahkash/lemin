/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   graph_init.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtrisha <mtrisha@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/09 15:23:37 by mtrisha           #+#    #+#             */
/*   Updated: 2020/01/09 15:26:30 by mtrisha          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <farm.h>

#include <input_errors.h>

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

static int				dnbr_cmp(const void *restrict number_1,
									const void *restrict number_2)
{
	int	res;

	res = ((t_dnbr *)number_1)->a - ((t_dnbr *)number_2)->a;
	return (res ? res : (((t_dnbr *)number_1)->b - ((t_dnbr *)number_2)->b));
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
