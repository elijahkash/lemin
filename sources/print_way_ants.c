/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_way_ants.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmathew <hmathew@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/10 14:01:41 by hmathew           #+#    #+#             */
/*   Updated: 2019/12/10 14:47:31 by hmathew          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "farm.h"
#include "libft.h"
#include "print_way_ants.h"

void	print_result(t_enum_ways *restrict combs, t_farm *restrict farm)
{
	ft_printf("ways = %d\n", combs->count);
	t_darr	test;
	darr_init(&test, 4, 256);
	for (t_uint j = 0; j < combs->count; j++)
	{
		ft_printf("[%s]", *(char **)vect(&(farm->names), farm->graph.start));
		for(t_uint i = 0; i < combs->ways[j].len; i++)
		{
			ft_printf("->[%s]", *(char **)vect(&(farm->names), combs->ways[j].nodes[i]));
			if (combs->ways[j].nodes[i] != farm->graph.end)
			{
				for(int k = 0; k < (int)darr_l(test); k++)
					if (combs->ways[j].nodes[i] == *(t_uint *)darr(test, k))
						ft_printf("*");
			}
			darr_add(test, &(combs->ways[j].nodes[i]));
		}
		ft_printf("\n");
	}
	ft_printf("\n");
	return ;
}
