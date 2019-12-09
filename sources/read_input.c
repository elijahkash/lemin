/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_input.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtrisha <mtrisha@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/09 12:56:19 by mtrisha           #+#    #+#             */
/*   Updated: 2019/12/09 15:37:15 by mtrisha          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <read_input.h>

#include <libft.h>
#include <input_errors.h>

static int		read_tube(int state, char *restrict line,
							t_farm *restrict farm)
{
	size_t count;
	t_uint	i;
	t_uint	j;
	char	*tmp;

	count = ft_ccwords(line, '-');
	if (count == 2)
	{
		tmp = ft_strchr(line, '-');
		*(tmp++) = '\0';
		i = vect_flfind_i(&(farm->names), &line, ft_scmp);
		j = vect_flfind_i(&(farm->names), &tmp, ft_scmp);
		state |= (i-- * j-- == 0) ? ERRSTATE | TUBE_ERROR : 0;
		if (i != j)
		{
			mtrx_set(farm, i, j);
			mtrx_set(farm, j, i);
		}
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
	if (line[0] == 'L' ||
		((tmp = ft_strchr(line, '-')) && tmp < (line = ft_strchr(line, ' '))))
		return (0);
	line = ft_skip_atoi(line + 1);
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
		vect_trim(farm->names);
		vect_trim(farm->chars);
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
		state |= (state & (START | END) || farm->start != 0 - 1) ?
					(ERRSTATE | WRONG_CMD) : START;
	else if (!ft_strcmp(line, "##end"))
		state |= (state & (START | END) || farm->end != -1) ?
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
