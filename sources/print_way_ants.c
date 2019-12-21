/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_way_ants.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtrisha <mtrisha@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/10 14:01:41 by hmathew           #+#    #+#             */
/*   Updated: 2019/12/21 22:08:11 by mtrisha          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "farm.h"
#include "libft.h"
#include "print_way_ants.h"

/*
**	create array ants_names in t_enum_ways & fill him a NO_ANT values
*/

void	init_ways_antsnames(t_enum_ways *restrict eways)
{
	t_uint i;
	t_uint j;

	i = 0;
	while (i < eways->count)
	{
		eways->ways[i].ants_names = ft_malloc(sizeof(int) * eways->ways[i].len);
		eways->ways[i].last_ant = NO_ANT;
		j = 0;
		while (j < eways->ways[i].len)
		{
			eways->ways[i].ants_names[j] = NO_ANT;
			j++;
		}
		i++;
	}
}

/*
**	Print ant's moves in the way
*/

void	print_move_in_way(t_way *way, t_farm *restrict farm, int *new_ant_name)
{
	int j;

	j = way->last_ant + 1;
	while (--j >= 0 && way->ants_names[j] != NO_ANT)
	{
		if (j != (int)way->len - 1)
		{
			ft_printf("L%d-%s ", way->ants_names[j],
					  *(char **)vect(&(farm->names), way->nodes[j + 1]));
			way->ants_names[j + 1] = way->ants_names[j];
			way->last_ant = (way->last_ant < j + 1) ? j + 1 : way->last_ant;
		}
		way->ants_names[j] = NO_ANT;
	}
	if (way->ants)
	{
		if (way->last_ant == NO_ANT)
			way->last_ant = 0;
		way->ants_names[0] = ++(*new_ant_name);
		way->ants--;
		ft_printf("L%d-%s ", (*new_ant_name),
			*(char **)vect(&(farm->names), way->nodes[0]));
	}
}

/*
**	Print ant's moves
*/

void	print_moves(t_enum_ways *restrict eways, t_farm *restrict farm)
{
	int i;
	int new_ant_name;

	init_ways_antsnames(eways);
	new_ant_name = 0;
	while (eways->moves)
	{
		i = 0;
		while (i < (int)eways->count)
		{
			print_move_in_way(&(eways->ways[i]), farm, &new_ant_name);
			i++;
		}
		eways->moves--;
		ft_printf("\n");
	}
}

void	print_ways(t_enum_ways *restrict eways, t_farm *restrict farm)
{
	t_darr	test;

	ft_printf("#Ways count = %d\n#Moves = %lu\n", eways->count, eways->moves);
	darr_init(&test, 4, 256);
	for (t_uint j = 0; j < eways->count; j++)
	{
		ft_printf("#len = %lu\tants = %lld\t", eways->ways[j].len,
												eways->ways[j].ants);
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
	ft_printf("#\n");
}

#ifdef DEBUG

void	print_result(t_enum_ways *restrict eways, t_farm *restrict farm)
{
	print_ways(eways, farm);
	print_moves(eways, farm);
	return ;
}

#else

void	print_result(t_enum_ways *restrict eways, t_farm *restrict farm)
{
	t_uint tmp;

	tmp = eways->moves;
	ft_printf("# Ways count = %d\n# Moves = %lu\n", eways->count, tmp);
	print_moves(eways, farm);
	return ;
}

#endif
