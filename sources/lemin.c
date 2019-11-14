/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lemin.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtrisha <mtrisha@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/06 16:20:37 by mtrisha           #+#    #+#             */
/*   Updated: 2019/11/14 20:46:20 by mtrisha          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <lemin.h>

#include <t_farm.h>
#include <handle_input.h>
#include <solve.h>
#include <prepare_work_graph.h>

void	lemin(void)
{
	t_farm	farm[1];
	int		ret;

	farm_init(farm);
	ret = handle_input(farm);
	if (ret)
		ft_printf("ERROR\n");
	else if (!farm->ants)
		ft_printf("There's no ants!\n");
	else if (!darr_l(farm->rooms))
		ft_printf("There's no rooms!\n");
	else if (farm->bcmtrx.mtrx == NULL)
		ft_printf("There's no tubes!\n");
	else if (farm->start == -1 || farm->end == -1)
		ft_printf("There's no start/end room!\n");
	if (ret || !(farm->ants * darr_l(farm->rooms)) || farm->start == -1 ||
			farm->end == -1 || !farm->bcmtrx.mtrx)
		return ;



	ft_printf("\n");

	int i;
	t_mtrx_iter iter;
	int k;
	i = 0;
	while (i < (int)darr_l(farm->rooms))
	{
		ft_printf("%s\t%d\t", *(char **)darr(farm->rooms, i),
								mtrx_getcon(farm, i));
		mtrx_iter_init(&iter, farm, i);
		while ((k = mtrx_next(&iter, farm)) >= 0)
			ft_printf("%d ", k);
		ft_printf("\n");
		i++;
	}

	ft_printf("\n\n%d\n\n", i);


	ret = prepare_work_graph(farm);
	if (ret)
		ft_printf("There's no way between start and end!\n");
	solve(farm);



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


	t_iter iter2;
	i = 0;
	t_connect *ptr;
	while (i < farm->work_graph.size)
	{
		ft_printf("%s\t%d\t", *(char **)darr(farm->rooms, GRAPH_ITEM(i).id),
								GRAPH_ITEM(i).con_count);
		iter_init(&iter2, i);
		while ((ptr = next(&iter2, farm)))
		{
			ft_printf("%d ", ptr->dst);
		}
		ft_printf("\n");
		i++;
	}

	ft_printf("\n\n%d\n\n", i);

	return ;
}
