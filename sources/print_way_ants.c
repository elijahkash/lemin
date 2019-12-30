/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_way_ants.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtrisha <mtrisha@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/10 14:01:41 by hmathew           #+#    #+#             */
/*   Updated: 2019/12/30 20:06:34 by mtrisha          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "farm.h"
#include "libft.h"
#include "print_way_ants.h"

void	init_ways_antsnames(t_enum_ways *restrict eways)
{
	t_uint	i;
	t_uint	size;

	i = (t_uint)-1;
	size = 0;
	while (++i < eways->count)
		size += eways->ways[i].len;
	eways->ants_mem = ft_memalloc(sizeof(t_uint) * size);
	i = (t_uint)-1;
	size = 0;
	while (++i < eways->count)
	{
		eways->ways[i].first_ant = 0;
		eways->ways[i].ants_on_way = 0;
		eways->ways[i].ants_names = eways->ants_mem + size/* * sizeof(t_uint)*/;
		size += eways->ways[i].len;
	}
}

void	print_ant_move(t_int ant_name, char *room_name, t_outbuff *outbuff)
{
	char	nbr[11];

	ft_getunbr_goodbase(ant_name, "0123456789", 10, nbr);
	ft_buff_add_to_outbuff(outbuff, "L", 1);
	ft_buff_add_to_outbuff(outbuff, nbr, ft_strlen(nbr));
	ft_buff_add_to_outbuff(outbuff, "-", 1);
	ft_buff_add_to_outbuff(outbuff, room_name, ft_strlen(room_name));
	ft_buff_add_to_outbuff(outbuff, " ", 1);
}

void	print_move_in_way(t_way *restrict way, t_farm *restrict farm,
							t_uint *new_ant_name, t_outbuff *outbuff)
{
	t_int	i;

	if (way->ants_on_way == 0 && way->ants == 0)
		return ;
	ft_memmove(&(way->ants_names[way->first_ant + 1]),
		&(way->ants_names[way->first_ant]), way->ants_on_way * sizeof(t_uint));
	i = way->first_ant + way->ants_on_way + 1;
	while (--i > way->first_ant)
		print_ant_move(way->ants_names[i],
						*(char **)vect(&(farm->names), way->nodes[i]), outbuff);
	way->first_ant++;
	(way->first_ant + way->ants_on_way == way->len) ? way->ants_on_way-- : 0;
	if (way->ants)
	{
		way->ants_names[0] = ++(*new_ant_name);
		way->ants--;
		way->ants_on_way++;
		way->first_ant = 0;
		print_ant_move(way->ants_names[0],
						*(char **)vect(&(farm->names), way->nodes[0]), outbuff);
	}
}

void	print_moves(t_enum_ways *restrict eways, t_farm *restrict farm)
{
	t_uint		i;
	t_uint		new_ant_name;
	t_outbuff	*outbuff;

	outbuff = ft_get_outbuff_item(1);
	init_ways_antsnames(eways);
	new_ant_name = 0;
	while (eways->moves--)
	{
		i = (t_uint)-1;
		while (++i < eways->count)
			print_move_in_way(&(eways->ways[i]), farm, &new_ant_name, outbuff);
		ft_buff_add_to_outbuff(outbuff, "\n", 1);
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
//	print_ways(eways, farm);
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
