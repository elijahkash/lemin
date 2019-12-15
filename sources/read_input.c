/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_input.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtrisha <mtrisha@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/09 12:56:19 by mtrisha           #+#    #+#             */
/*   Updated: 2019/12/15 13:56:11 by mtrisha          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <read_input.h>

#include <libft.h>
#include <input_errors.h>

static t_uint		read_tube(t_uint state, char *restrict line,
								t_farm *restrict farm)
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

static t_uint		is_uniq_names(t_vect *restrict names)
{
	size_t	i;
	size_t	is_error;
	size_t	size;

	is_error = 0;
	size = names->curlen - 1;
	i = 0;
	while (i < size)
	{
		is_error += ft_strcmp(*(char **)vect(names, i),
							*(char **)vect(names, i + 1)) ? 0 : 1;
		i++;
	}
	return (is_error ? 0 : 1);
}

static void			form_rooms(t_farm *restrict farm)
{
	size_t	i;
	char	*ptr;

	i = 0;
	while (i < farm->names.curlen)
	{
		ptr = vect(&(farm->chars), *(size_t *)vect(&(farm->names), i));
		vect_eq(&(farm->names), i, ft_p(ptr));
		i++;
	}
}

static t_uint		is_room(char *restrict line)
{
	char *tmp_1;
	char *tmp_2;

	tmp_1 = ft_strchr(line, '-');
	tmp_2 = ft_strchr(line, ' ');
	if (line[0] == 'L' || (tmp_1 && tmp_1 < tmp_2))
		return (0);
	line = tmp_2 + 1;
	line = ft_skip_atoi(line);
	line += (*line == ' ') ? 1 : 0;
	line = ft_skip_atoi(line);
	line = ft_skip_atoi(line);
	return (*line ? 0 : 1);
}

static t_uint		read_room(t_uint state, char *restrict line,
								t_farm *restrict farm)
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
		form_rooms(farm);
		vect_sort(&(farm->names), ft_scmp, ft_qsort);
		state = is_uniq_names(&(farm->names)) ?
				(state & ~ROOMS) | TUBES : (state | ERRSTATE | NO_UNIQ);
		farm_init_connects(farm);
	}
	else
		state |= ERRSTATE | ROOM_ERROR;
	return (state);
}

static t_uint		read_ants(t_uint state, char *restrict line,
								t_farm *restrict farm)
{
	if (state & (START | END) || ft_isdigit_ws(line))
		return (state | ERRSTATE | ANTS_ERROR);
	farm->ants = ft_atoi(line);
	state &= ~ANTS;
	state |= ROOMS;
	farm_init_rooms(farm);
	return (state);
}

static t_uint		handle_cmd(t_uint state, char *restrict line,
								t_farm *restrict farm)
{
	if (!ft_strcmp(line, "##start"))
		state |= (state & (START | END) || farm->start != FARM_INIT_SE_VALUES) ?
					(ERRSTATE | WRONG_CMD) : START;
	else if (!ft_strcmp(line, "##end"))
		state |= (state & (START | END) || farm->end != FARM_INIT_SE_VALUES) ?
					(ERRSTATE | WRONG_CMD) : END;
	return (state);
}

static t_uint		handle_line(char *restrict line, t_farm *restrict farm)
{
	static t_uint state = ANTS;

	if (line[0] == '#' && line[1] == '#')
		state = handle_cmd(state, line, farm);
	else if (*line == '#')
		return (0);
	else if (state & ANTS)
		state = read_ants(state, line, farm);
	else if (state & ROOMS)
		state = (farm->names.curlen == MAX_NODES) ? (ERRSTATE | TOO_MUCH) :
				read_room(state, line, farm);
	if (!(state & ERRSTATE) && state & TUBES)
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
