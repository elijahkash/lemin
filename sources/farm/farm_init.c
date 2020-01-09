/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   farm_init.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtrisha <mtrisha@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/09 15:26:59 by mtrisha           #+#    #+#             */
/*   Updated: 2020/01/09 15:27:39 by mtrisha          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <farm.h>

void					farm_init(t_farm *restrict farm)
{
	farm->ants = 0;
	farm->start = FARM_INIT_SE_VALUES;
	farm->end = FARM_INIT_SE_VALUES;
	farm->connects.mem = NULL;
	farm->chars.mem = NULL;
	farm->graph.mem = NULL;
}

void					farm_init_rooms(t_farm *restrict farm)
{
	vect_init(&(farm->chars), sizeof(char), FARM_INIT_CHARS_COUNT);
	vect_init(&(farm->names), sizeof(char *), FARM_INIT_ROOM_COUNT);
}

void					farm_init_connects(t_farm *restrict farm)
{
	vect_init(&(farm->connects), sizeof(t_dnbr),
				(size_t)(farm->names.curlen * FARM_INIT_CONNECTS_PER_ROOM));
}
