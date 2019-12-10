/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_way_ants.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmathew <hmathew@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/10 14:01:41 by hmathew           #+#    #+#             */
/*   Updated: 2019/12/10 18:19:53 by hmathew          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "farm.h"
#include "libft.h"
#include "print_way_ants.h"

/*
**	create array ants_names in t_enum_ways & fill him a NO_ANT values
*/
void	init_ways(t_enum_ways *restrict eways)
{
	int i;
	int j;

	i = 0;
	while (i < eways->count)
	{
		eways->ways[i].ants_names = ft_malloc(sizeof(t_uint) * eways->ways[i].len);
		j = 0;
		while (j < eways->ways[i].len)
		{
			eways->ways[i].ants_names[j] = NO_ANT;
			j++;
		}
		i++;
	}
}

void	print_lines(t_enum_ways *restrict eways, t_farm *restrict farm)
{
	int i;
	int j;
	int new_ant_name;
	int cnt_finish;

	new_ant_name = 0;
	cnt_finish = 0;

	while (eways->moves)
	{
		//ft_printf("moves = %d", eways->moves);
		i = 0;
		while (i < eways->count)
		{
			j = eways->ways[i].len;
			while (--j >= 0)
			{
				if (eways->ways[i].ants_names[j] != NO_ANT)
				{
					if (j != eways->ways[i].len - 1)
					{
						eways->ways[i].ants_names[j + 1] = eways->ways[i].ants_names[j];
						ft_printf("L%d[%s] ", eways->ways[i].ants_names[j], *(char **)vect(&(farm->names), eways->ways[i].nodes[j]));
					}
					else
						cnt_finish++;
					eways->ways[i].ants_names[j] = NO_ANT;
				}
				j--;
			}
			if (eways->ways[i].ants)
			{
				eways->ways[i].ants_names[0] = ++new_ant_name;
				eways->ways[i].ants--;
				ft_printf("L%d[%s] ", new_ant_name, *(char **)vect(&(farm->names), eways->ways[i].nodes[0]));
			}
			i++;
		}
		eways->moves--;
		ft_printf("\n");
	}
}


void	print_result(t_enum_ways *restrict eways, t_farm *restrict farm)
{
	ft_printf("ways = %d\n", eways->count);
	t_darr	test;
	darr_init(&test, 4, 256);
	for (t_uint j = 0; j < eways->count; j++)
	{
		ft_printf("[%s]", *(char **)vect(&(farm->names), farm->graph.start));
		for(t_uint i = 0; i < eways->ways[j].len; i++)
		{
			ft_printf("->[%s]", *(char **)vect(&(farm->names),
				eways->ways[j].nodes[i]));
			if (eways->ways[j].nodes[i] != farm->graph.end)
			{
				for(int k = 0; k < (int)darr_l(test); k++)
					if (eways->ways[j].nodes[i] == *(t_uint *)darr(test, k))
						ft_printf("*");
			}
			darr_add(test, &(eways->ways[j].nodes[i]));
		}
		ft_printf("\n");
	}
	ft_printf("\n");
	init_ways(eways);
	print_lines(eways, farm);
	return ;
}
