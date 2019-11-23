/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lemin.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtrisha <mtrisha@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/06 16:20:37 by mtrisha           #+#    #+#             */
/*   Updated: 2019/11/23 18:39:22 by mtrisha          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <lemin.h>

#include <t_farm.h>
#include <handle_input.h>
#include <solve.h>

void	lemin(void)
{
	t_source_farm	*src_farm;
	t_work_farm		*work_farm;
	int				ret;

	src_farm_init(&src_farm);
	ret = handle_input(src_farm);
	if (ret)
		ft_printf("ERROR\n");
	else if (!src_farm->ants)
		ft_printf("There's no ants!\n");
	else if (!darr_l(src_farm->rooms))
		ft_printf("There's no rooms!\n");
	else if (src_farm->bcmtrx.mtrx == NULL)
		ft_printf("There's no tubes!\n");
	else if (src_farm->start == -1 || src_farm->end == -1)
		ft_printf("There's no start/end room!\n");
	if (ret || !(src_farm->ants * darr_l(src_farm->rooms)) ||
		src_farm->start == -1 || src_farm->end == -1 || !src_farm->bcmtrx.mtrx)
		return ;
	ft_force_buff();


	ft_printf("\n");

	__int32_t i;
	t_mtrx_iter iter;
	__int32_t k;
	i = 0;
	while (i < (int)darr_l(src_farm->rooms))
	{
		ft_printf("%s\t%d\t", *(char **)darr(src_farm->rooms, i),
								mtrx_getcon(src_farm, i));
		mtrx_iter_init(&iter, src_farm, i);
		while ((k = mtrx_next(&iter, src_farm)) >= 0)
			ft_printf("%d ", k);
		ft_printf("\n");
		i++;
	}

	ft_printf("\n\n%d\n\n", i);


	work_farm_init(&work_farm, src_farm);
	src_farm_del(&src_farm);
	if (!work_farm->graph.mem)
		ft_printf("There's no way between start and end!\n");
	solve(work_farm);


	// int j;
	// i = 0;
	// while (i < (int)darr_l(farm.rooms))
	// {
	// 	j = 0;
	// 	ft_printf("%s\t%d\t", *(char **)darr(farm.rooms, i),
	// 							mtrx_getcon(&farm, i));
	// 	while (j < (int)darr_l(farm.rooms))
	// 		ft_printf("%d ", mtrx(&farm, i, j++));
	// 	ft_printf("\n");
	// 	i++;
	// }


	ft_printf("\n");


	t_graph_iter iter2;
	t_connect *ptr;
	i = 0;
	while (i < work_farm->graph.size)
	{
		ft_printf("%d\t%s\t%d\t", i, (char *)darr(work_farm->rooms,
			GRAPH_ITEM(work_farm, i)->id), GRAPH_ITEM(work_farm, i)->con_count);
		graph_iter_init(&iter2, i, 0);
		while ((ptr = graph_next(&iter2, work_farm)))
		{
			ft_printf("%d ", ptr->dst * ((ptr->state & WAY_NEGATIVE) ? -1 : 1));
		}
		ft_printf("\n");
		i++;
	}
	ft_printf("\n\n%d\n\n", i);


	// t_connect test;
	// test.dst = 2;
	// test.weight = 1;
	// graph_reset(work_farm, 4, 2);
	// graph_reset(work_farm, 2, 5);
	// graph_set(work_farm, 4, test);


	// i = 0;
	// while (i < work_farm->graph.size)
	// {
	// 	ft_printf("%s\t%d\t", (char *)darr(work_farm->rooms,
	// 		GRAPH_ITEM(work_farm, i)->id), GRAPH_ITEM(work_farm, i)->con_count);
	// 	graph_iter_init(&iter2, i);
	// 	while ((ptr = graph_next(&iter2, work_farm)))
	// 	{
	// 		ft_printf("%d ", ptr->dst);
	// 	}
	// 	ft_printf("\n");
	// 	i++;
	// }
	// ft_printf("\n\n%d\n\n", i);

	 ft_force_buff();




	work_farm_del(&work_farm);
	return ;
}
