/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_result.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Kashnitskiy <elijahkash.code@gmail.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/10 14:01:41 by hmathew           #+#    #+#             */
/*   Updated: 2020/01/27 17:06:58 by Kashnitskiy      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ways.h>

#include <libft.h>

static void		init_ways_antsnames(t_enum_ways *restrict eways)
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
		eways->ways[i].ants_names = eways->ants_mem + size;
		size += eways->ways[i].len;
	}
}

static void		print_ant_move(t_int ant_name, char *restrict room_name,
								t_outbuff *restrict outbuff)
{
	char	nbr[11];

	ft_getunbr_goodbase(ant_name, "0123456789", 10, nbr);
	ft_buff_add_to_outbuff(outbuff, "L", 1);
	ft_buff_add_to_outbuff(outbuff, nbr, ft_strlen(nbr));
	ft_buff_add_to_outbuff(outbuff, "-", 1);
	ft_buff_add_to_outbuff(outbuff, room_name, ft_strlen(room_name));
	ft_buff_add_to_outbuff(outbuff, " ", 1);
}

static void		print_move_in_way(t_way *restrict way, t_farm *restrict farm,
					t_uint *restrict new_ant_name, t_outbuff *restrict outbuff)
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

void			print_result(t_enum_ways *restrict eways, t_farm *restrict farm)
{
	t_uint		i;
	t_uint		new_ant_name;
	t_outbuff	*outbuff;

	ft_printf("#> Ways count = %d\n#> Moves = %lu\n",
				eways->count, eways->moves);
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
