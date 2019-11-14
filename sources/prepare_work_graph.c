/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prepare_work_graph.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtrisha <mtrisha@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/13 18:28:41 by mtrisha           #+#    #+#             */
/*   Updated: 2019/11/14 20:57:27 by mtrisha          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <prepare_work_graph.h>

void	determ_connect_component(t_darr connect_component, t_farm *farm)
{
	int i;
	int j;
	t_mtrx_iter iter;
	t_darr		supp;

	i = 0;
	darr_init(&supp, sizeof(int), 128);
	darr_add(connect_component, &(farm->start));
	darr_add(supp, &(farm->start));
	while (i < (int)darr_l(connect_component))
	{
		if (*(int *)darr(connect_component, i) == farm->end)
		{
			i++;
			continue ;
		}
		mtrx_iter_init(&iter, farm, *(int *)darr(connect_component, i));
		while ((j = mtrx_next(&iter, farm)) >= 0)
		{
			if (!darr_flfind_i(supp, &j, ft_icmp))
			{
				darr_insert_uniq(supp, &j, ft_icmp);
				darr_add(connect_component, &j);
			}
		}
		i++;
	}
	darr_del(&supp);
}

void	del_dead_end(t_farm *farm, t_darr connect_component)
{
	int		i;
	int		j;
	t_mtrx_iter	iter;
	int		tmp;

	i = darr_l(connect_component);
	while (i-- > 0)
	{
		tmp = *(int *)darr(connect_component, i);
		if (mtrx_getcon(farm, tmp) == 1 &&
			tmp != farm->start && tmp != farm->end)
		{
			mtrx_iter_init(&iter, farm, tmp);
			j = mtrx_next(&iter, farm);
			mtrx_reset(farm, tmp, j);
			mtrx_reset(farm, j, tmp);
			darr_pop_i(connect_component, i);
		}
	}
}

int		count_connections(t_farm *farm, t_darr connect_component)
{
	int		res;
	int 	i;

	res = 0;
	i = 0;
	while (i < (int)darr_l(connect_component))
	{
		res += mtrx_getcon(farm, *(int *)darr(connect_component, i));
		i++;
	}
	return (res);
}

void	fill_connections(t_connect *arr, t_farm *farm, int i,
							t_darr connect_component)
{
	int		k;
	int		j;
	t_mtrx_iter	iter;

	mtrx_iter_init(&iter, farm, i);
	j = 0;
	while ((k = mtrx_next(&iter, farm)) >= 0)
	{
		arr[j].dst = darr_flfind_i(connect_component, &k, ft_icmp) - 1;
		arr[j].weight = 1;
		j++;
	}
}

void	create_work_graph(t_farm *farm, t_darr connect_component)
{
	int		i;
	int		connects;
	char	*bias;

	i = 0;
	bias = 0;
	farm->work_graph.size = (int)darr_l(connect_component);
	farm->work_graph.mem = ft_malloc(
			farm->work_graph.size * (PTR_SIZE + sizeof(t_graph_item)) +
			sizeof(t_connect) * count_connections(farm, connect_component));
	bias = (char *)farm->work_graph.mem + farm->work_graph.size * PTR_SIZE;
	while (i < farm->work_graph.size)
	{
		connects = mtrx_getcon(farm, *(int *)darr(connect_component, i));
		((void **)farm->work_graph.mem)[i] = bias;
		GRAPH_ITEM(i).id = *(int *)darr(connect_component, i);
		GRAPH_ITEM(i).con_count = connects;
		GRAPH_ITEM(i).state = 0;
		fill_connections((t_connect *)(bias + sizeof(t_graph_item)), farm,
							GRAPH_ITEM(i).id, connect_component);
		bias += sizeof(t_graph_item) + sizeof(t_connect) * connects;
		i++;
	}
}

int		prepare_work_graph(t_farm *farm)
{
	t_darr	connect_component;

	darr_init(&connect_component, sizeof(int), 64);
	determ_connect_component(connect_component, farm);
	del_dead_end(farm, connect_component);
	darr_sort(connect_component, ft_icmp, ft_qsort);
	if (!darr_flfind_i(connect_component, &(farm->end), ft_icmp))
		return (1);
	create_work_graph(farm, connect_component);
	darr_del(&connect_component);
	return (0);
}
