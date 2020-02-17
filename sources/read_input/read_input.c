/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_input.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Kashnitskiy <elijahkash.code@gmail.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/09 12:56:19 by mtrisha           #+#    #+#             */
/*   Updated: 2020/02/17 10:48:44 by Kashnitskiy      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <read_input.h>

#include <input_utils_basic.h>
#include <input_errors.h>
#include <libft.h>

static t_uint		handle_line(char *restrict line, t_farm *restrict farm)
{
	static t_uint state = ANTS;

	if (line[0] == '#' && line[1] == '#')
	{
		state = handle_cmd(state, line, farm);
		return (state & ~NO_ERROR);
	}
	else if (*line == '#')
		return (0);
	else if (state & ANTS)
		state = read_ants(state, line, farm);
	else if (state & ROOMS)
		state = (farm->names.curlen == MAX_NODES) ? (ERRSTATE | TOO_MUCH) :
				read_room(state, line, farm);
	if (((state & ERRSTATE) == 0) && state & TUBES)
		state = read_tube(state, line, farm);
	return ((state & ~NO_ERROR));
}

t_uint				read_input(t_farm *restrict farm)
{
	int			gnl_ret;
	t_uint		ret;
	char		*line;
	t_outbuff	*buff;

	buff = ft_get_outbuff_item(FD_STDOUT);
	while ((gnl_ret = ft_fast_input_gnl(&line)))
	{
		if (gnl_ret < 0 || !line)
		{
			ft_memdel((void **)&line);
			return (GNL_ERROR);
		}
		ret = handle_line(line, farm);
		ft_buff_add_to_outbuff(buff, line, ft_strlen(line));
		ft_buff_add_to_outbuff(buff, "\n", 1);
		ft_free(line);
		if (ret)
			return (ret);
	}
	ft_free(line);
	if (farm->connects.mem)
		vect_shrink(&(farm->connects), 0);
	return (0);
}
