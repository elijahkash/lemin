/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_utils_basic.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Kashnitskiy <elijahkash.code@gmail.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/09 12:58:06 by mtrisha           #+#    #+#             */
/*   Updated: 2020/01/27 17:07:08 by Kashnitskiy      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <input_utils_basic.h>

#include <input_errors.h>
#include <input_utils_supp.h>

t_uint		read_tube(t_uint state, char *restrict line, t_farm *restrict farm)
{
	size_t	count;
	t_dnbr	connect;
	char	*tmp;

	count = ft_ccwords(line, '-');
	if (count == 2)
	{
		tmp = ft_strchr(line, '-');
		*(tmp++) = '\0';
		connect.a = vect_bin_find(&(farm->names), (void *)&line, ft_scmp);
		connect.b = vect_bin_find(&(farm->names), (void *)&tmp, ft_scmp);
		*(--tmp) = '-';
		state |= (connect.a-- * connect.b-- == 0) ? ERRSTATE | TUBE_ERROR : 0;
		if (connect.a == connect.b)
			return (state);
		if (connect.a > connect.b)
			ft_swap(&(connect.a), &(connect.b), sizeof(t_uint));
		vect_add(&(farm->connects), &connect);
	}
	else
		state |= ERRSTATE | TUBE_ERROR;
	return (state);
}

t_uint		read_room(t_uint state, char *restrict line, t_farm *restrict farm)
{
	size_t count;

	count = ft_ccwords(line, ' ');
	if ((count == 3 || count == 4) && is_room(line))
	{
		vect_add(&(farm->names), ft_z(farm->chars.curlen));
		farm->start = (state & START) ? farm->chars.curlen : farm->start;
		farm->end = (state & END) ? farm->chars.curlen : farm->end;
		state &= ~(START | END);
		vect_add_n(&(farm->chars), line, ft_strchr(line, ' ') - line);
		vect_add(&(farm->chars), ft_c('\0'));
	}
	else if (count == 1)
	{
		vect_shrink(&(farm->names), 0);
		vect_shrink(&(farm->chars), 0);
		farm->names.curlen ? form_rooms(farm) : (state |= ERRSTATE | NO_ROOMS);
		vect_sort(&(farm->names), ft_scmp, ft_qsort);
		state = (is_uniq_names(&(farm->names))) ?
				(state & ~ROOMS) | TUBES : (state | ERRSTATE | NO_UNIQ);
		((state & ERRSTATE) == 0) ? farm_init_connects(farm) : 0;
	}
	else
		state |= ERRSTATE | ROOM_ERROR;
	return (state);
}

t_uint		read_ants(t_uint state, char *restrict line, t_farm *restrict farm)
{
	if (state & (START | END) || *ft_skip_atoi(line) || ft_strchr(line, '-'))
		return (state | ERRSTATE | ANTS_ERROR);
	farm->ants = ft_atoi_l(line);
	if (ft_strlen(line) > 11 || farm->ants > __UINT32_MAX__)
		return (state | ERRSTATE | ANTS_ERROR);
	state &= ~ANTS;
	state |= ROOMS;
	farm_init_rooms(farm);
	return (state);
}

t_uint		handle_cmd(t_uint state, char *restrict line, t_farm *restrict farm)
{
	if (!ft_strcmp(line, "##start"))
		state |= (state & (START | END) || farm->start != FARM_INIT_SE_VALUES) ?
					(ERRSTATE | WRONG_CMD) : START;
	else if (!ft_strcmp(line, "##end"))
		state |= (state & (START | END) || farm->end != FARM_INIT_SE_VALUES) ?
					(ERRSTATE | WRONG_CMD) : END;
	return (state);
}
