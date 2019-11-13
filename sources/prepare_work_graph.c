/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prepare_work_graph.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtrisha <mtrisha@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/13 18:28:41 by mtrisha           #+#    #+#             */
/*   Updated: 2019/11/13 21:46:05 by mtrisha          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <prepare_work_graph.h>

void	determ_connect_component(t_darr connect_component, t_farm *farm)
{
	int i;
	int j;
	t_iter iter;

	i = 0;
	darr_add(connect_component, &(farm->start));
	while (i < darr_l(connect_component))
	{
		if (*(int *)darr(connect_component, i) == farm->end)
		{
			i++;
			continue ;
		}
		iter_init(&iter, &farm, *(int *)darr(connect_component, i));
		while ((j = next(&iter, farm)) >= 0)
			darr_insert_uniq(connect_component, &j, ft_icmp);
		i++;
	}
}

void	del_dead_end(t_farm *farm, t_darr connect_component)
{
	int		i;
	int		j;
	t_iter	iter;
	int		tmp;

	i = darr_l(connect_component);
	while (i-- > 0)
	{
		tmp = *(int *)darr(connect_component, i);
		if (mtrx_getcon(farm, tmp) == 1)
		{
			iter_init(&iter, &farm, tmp);
			j = next(&iter, farm);
			mtrx_reset(farm, tmp, j);
			mtrx_reset(farm, j, tmp);
			darr_pop_i(connect_component, i);
		}
	}
}

void	create_work_graph(t_farm *farm, t_darr connect_component)
{
	farm->work_graph.size = (int)darr_l(connect_component);
	farm->work_graph.mem = ft_malloc(
			farm->work_graph.size * (sizeof(void *) + sizeof(t_graph_item)) +
			sizeof(t_connect) * count_connections(farm, connect_component));
	//TODO: from here
}

int		prepare_work_graph(t_farm *farm)
{
	t_darr	connect_component;

	darr_init(&connect_component, sizeof(int), 64);
	determ_connect_component(connect_component, &farm);
	if (!darr_flfind_i(connect_component, &(farm->end), ft_icmp))
		return (1);
	del_dead_end(farm, connect_component);
	create_work_graph(farm, connect_component);
	darr_del(&connect_component);
	return (0);
}
