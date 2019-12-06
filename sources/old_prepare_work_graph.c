/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   old_prepare_work_graph.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtrisha <mtrisha@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/13 18:28:41 by mtrisha           #+#    #+#             */
/*   Updated: 2019/12/06 11:24:02 by mtrisha          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <old_prepare_work_graph.h> //TODO: rename file?

void	determ_connect_component(t_darr connect_component, t_source_farm *farm)
{
	int			i;
	int			j;
	t_mtrx_iter	iter;
	t_darr		supp;

	darr_init(&supp, sizeof(int), darr_l(farm->rooms));
	darr_add(connect_component, &(farm->start));
	darr_add(supp, &(farm->start));
	i = -1;
	while (++i < (int)darr_l(connect_component))
	{
		mtrx_iter_init(&iter, farm, *(int *)darr(connect_component, i));
		while ((j = mtrx_next(&iter, farm)) >= 0)
		{
			if (darr_insert_uniq(supp, &j, ft_icmp))
				darr_add(connect_component, &j);
		}
	}
	darr_del(&supp);
}

void	del_dead_end(t_source_farm *farm, t_darr connect_component)
{
	int			i;
	int			j;
	t_mtrx_iter	iter;
	int			tmp;

	i = darr_l(connect_component);
	while (--i > 0)
	{
		tmp = *(int *)darr(connect_component, i);
		if (mtrx_getcon(farm, tmp) == 1 && tmp != farm->end)
		{
			mtrx_iter_init(&iter, farm, tmp);
			j = mtrx_next(&iter, farm);
			mtrx_reset(farm, tmp, j);
			mtrx_reset(farm, j, tmp);
			darr_pop_i(connect_component, i);
		}
	}
}

int		count_connections(t_source_farm *farm, t_darr connect_component)
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

void	fill_connections(t_connect *arr, t_source_farm *farm, int i,
							t_darr connect_component)
{
	int		k;
	int		j;
	t_mtrx_iter	iter;

	mtrx_iter_init(&iter, farm, i);
	j = 0;
	while ((k = mtrx_next(&iter, farm)) >= 0)
	{
		arr[j].dst = darr_flfind_i(connect_component, &k, ft_icmp) - 1; // -1
		// arr[j].state = (farm->end != i) ? WAY_INIT_STATE : END_WAY; // del from here!!!!!!!!!!!!!!!!!!!!!!!
		arr[j].state = WAY_BASE_STATE;
		j++;
	}
}

void	create_graph(t_work_farm *work_farm, t_source_farm *src_farm,
					t_darr connect_component)
{
	int		i;
	int		connects;
	char	*bias;

	i = 0;
	bias = 0;
	work_farm->graph.size = (int)darr_l(connect_component);
	work_farm->graph.mem = ft_malloc(
			work_farm->graph.size * (PTR_SIZE + sizeof(t_graph_item)) +
			sizeof(t_connect) * count_connections(src_farm, connect_component));
	bias = (char *)work_farm->graph.mem + work_farm->graph.size * PTR_SIZE;
	while (i < work_farm->graph.size)
	{
		connects = mtrx_getcon(src_farm, *(int *)darr(connect_component, i));
		((void **)work_farm->graph.mem)[i] = bias;
		GRAPH_ITEM(work_farm, i)->id = *(int *)darr(connect_component, i);
		GRAPH_ITEM(work_farm, i)->con_count = connects;
		GRAPH_ITEM(work_farm, i)->state = INIT_STATE;
		GRAPH_ITEM(work_farm, i)->weight = INIT_WEIGHT;
		fill_connections((t_connect *)(bias + sizeof(t_graph_item)), src_farm,
							GRAPH_ITEM(work_farm, i)->id, connect_component);
		bias += sizeof(t_graph_item) + sizeof(t_connect) * connects;
		i++;
	}
}

void	restruct_names(t_work_farm *work_farm, t_source_farm *src_farm)
{
	int		i;
	char	*tmp;

	darr_init(&(work_farm->rooms), sizeof(char), work_farm->graph.size * 4);
	i = -1;
	while (++i < work_farm->graph.size)
	{
		tmp = *(char **)darr(src_farm->rooms, GRAPH_ITEM(work_farm, i)->id);
		GRAPH_ITEM(work_farm, i)->id = (int)darr_l(work_farm->rooms);
		darr_add_n(work_farm->rooms, tmp, ft_strlen(tmp) + 1);
	}
	darr_trim(work_farm->rooms);
}

void	del_extra_end_ways(t_source_farm *farm, t_darr connect_component)
{
	t_mtrx_iter	iter;
	__int32_t	i;

	mtrx_iter_init(&iter, farm, farm->end);
	while ((i = mtrx_next(&iter, farm)) >= 0)
		if (!darr_flfind_i(connect_component, &i, ft_icmp))
			mtrx_reset(farm, farm->end, i);
}

void	create_work_farm(t_work_farm *work_farm, t_source_farm *src_farm)
{
	t_darr	connect_component;
	//__int32_t	tmp;

	// tmp = src_farm->bcmtrx.mtrx[src_farm->end * src_farm->bcmtrx.mtrx_len];
	// src_farm->bcmtrx.mtrx[src_farm->end * src_farm->bcmtrx.mtrx_len] = 0;
	darr_init(&connect_component, sizeof(int), darr_l(src_farm->rooms));
	determ_connect_component(connect_component, src_farm);
	del_dead_end(src_farm, connect_component);
	darr_sort(connect_component, ft_icmp, ft_qsort);
	work_farm->end = darr_flfind_i(connect_component, &(src_farm->end),
										ft_icmp) - 1;
	if (work_farm->end == -1)
		return ;
	work_farm->start = darr_flfind_i(connect_component, &(src_farm->start),
											ft_icmp) - 1;
	// src_farm->bcmtrx.mtrx[src_farm->end * src_farm->bcmtrx.mtrx_len] = tmp;
	del_extra_end_ways(src_farm, connect_component);
	create_graph(work_farm, src_farm, connect_component);
	darr_del(&connect_component);
	restruct_names(work_farm, src_farm);
	return ;
}