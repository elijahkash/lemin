/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_input.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtrisha <mtrisha@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/09 12:56:19 by mtrisha           #+#    #+#             */
/*   Updated: 2019/12/09 20:42:12 by mtrisha          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <read_input.h>

#include <libft.h>
#include <input_errors.h>

static int		read_tube(int state, char *restrict line,
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

static int		is_uniq_names(t_vect *restrict names)
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

static void		form_rooms(t_farm *restrict farm)
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

static int		is_room(char *restrict line)
{
	char *tmp;

	//TODO: here
	if (line[0] == 'L' ||
		((tmp = ft_strchr(line, '-')) && tmp < ft_strchr(line, ' ')))
		return (0);
	line = ft_strchr(line, ' ') + 1;
	line = ft_skip_atoi(line);
	line += (*line == ' ') ? 1 : 0;
	line = ft_skip_atoi(line);
	return (*line ? 0 : 1);
}

static int		read_room(int state, char *restrict line,
							t_farm *restrict farm)
{
	size_t count;

	count = ft_ccwords(line, ' ');
	if (farm->names.curlen == MAX_NODES)
		return (ERRSTATE | TOO_MUCH);
	if (count == 1)
	{
		vect_shrink(&(farm->names), 0);
		vect_shrink(&(farm->chars), 0);
		form_rooms(farm);
		vect_sort(&(farm->names), ft_scmp, ft_qsort);
		state = is_uniq_names(&(farm->names)) ?
				(state & ~ROOMS) | TUBES : (state | ERRSTATE | NO_UNIQ);
		farm_init_connects(farm);
	}
	else if (count == 3 && is_room(line))
	{
		vect_add(&(farm->names), ft_z(farm->chars.curlen));
		farm->start = (state & START) ? farm->chars.curlen : farm->start;
		farm->end = (state & END) ? farm->chars.curlen : farm->end;
		state &= ~(START | END);
		vect_add_n(&(farm->chars), line, ft_strchr(line, ' ') - line);
		vect_add(&(farm->chars), ft_c('\0'));
	}
	else
		state |= ERRSTATE | ROOM_ERROR;
	return (state);
}

static int		read_ants(int state, char *restrict line,
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

static int		handle_cmd(int state, char *restrict line,
							t_farm *restrict farm)
{
	if (!ft_strcmp(line, "##start"))
		state |= (state & (START | END) || farm->start != (size_t)(0 - 1)) ?
					(ERRSTATE | WRONG_CMD) : START;
	else if (!ft_strcmp(line, "##end"))
		state |= (state & (START | END) || farm->end != (size_t)(0 - 1)) ?
					(ERRSTATE | WRONG_CMD) : END;
	return (state);
}

static int		handle_line(char *restrict line, t_farm *restrict farm)
{
	static int state = ANTS;

	if (!ft_strncmp(line, "##", 2))
		state = handle_cmd(state, line, farm);
	else if (*line == '#')
		return (0);
	else if (state & ANTS)
		state = read_ants(state, line, farm);
	else if (state & ROOMS)
		state = read_room(state, line, farm);
	if (!(state & ERRSTATE) && state & TUBES)
		state = read_tube(state, line, farm);
	return ((state & ~NO_ERROR));
}

int			read_input(t_farm *restrict farm)
{
	int		ret;
	char	*line;

	while ((ret = ft_get_next_line(0, &line)))
	{
		if (ret < 0 || !line)
		{
			ft_memdel((void **)&line);
			return (GNL_ERROR);
		}
		ret = handle_line(line, farm);
		ft_printf("%s\n", line);
		ft_free(line);
		if (ret)
			return (ret);
	}
	ft_free(line);
	return (0);
}
