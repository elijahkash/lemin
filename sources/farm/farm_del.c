/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   farm_del.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtrisha <mtrisha@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/09 15:28:11 by mtrisha           #+#    #+#             */
/*   Updated: 2020/01/09 15:28:27 by mtrisha          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <farm.h>

void					farm_del(t_farm *restrict farm)
{
	if (farm->chars.mem)
		farm_del_rooms(farm);
	if (farm->connects.mem)
		farm_del_connects(farm);
	if (farm->graph.mem)
		graph_del(&(farm->graph));
}

void					farm_del_connects(t_farm *restrict farm)
{
	vect_del(&(farm->connects));
}

void					farm_del_rooms(t_farm *restrict farm)
{
	vect_del(&(farm->chars));
	vect_del(&(farm->names));
}
