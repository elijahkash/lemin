/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   old_handle_input.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtrisha <mtrisha@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/07 12:56:25 by mtrisha           #+#    #+#             */
/*   Updated: 2019/12/09 14:23:53 by mtrisha          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <old_handle_input.h>

#include <limits.h>

static size_t	g_start = 0;
static size_t	g_end = 0;

static int		set_farm_se(t_source_farm *restrict farm)
{
	register size_t	i;
	register size_t size;

	size = darr_l(farm->rooms);
	if (size >= __INT32_MAX__ - 2)
		return (1);
	i = 0;
	while (i < size)
	{
		farm->start = (*(char **)darr(farm->rooms, i) ==
					darr(farm->names, g_start)) ? i : farm->start;
		farm->end = (*(char **)darr(farm->rooms, i)  ==
					darr(farm->names, g_end)) ? i : farm->end;
		if (i < size - 1 && !ft_strcmp(*(char **)darr(
			farm->rooms, i), *(char **)darr(farm->rooms, i + 1)))
			return (1);
		i++;
	}
	return (0);
}

static int		handle_cmd(int state, char *restrict line,
							t_source_farm *restrict farm)
{
	if (!ft_strcmp(line, "##start"))
		state |= (state & (START | END) || farm->start != -1) ? ERRSTATE :
																START;
	else if (!ft_strcmp(line, "##end"))
		state |= (state & (START | END) || farm->end != -1) ? ERRSTATE : END;
	return (state + (int)farm * 0);
}

static int		read_ants(int state, char *restrict line,
						t_source_farm *restrict farm)
{
	if (state & (START | END) || ft_isdigit_ws(line))
		return (state | ERRSTATE);
	farm->ants = ft_atoi(line);
	state &= ~ANTS;
	state |= ROOMS;
	return (state);
}

static int		read_tube(int state, char *restrict line,
						t_source_farm *restrict farm)
{
	char		**words;
	__int32_t	i;
	__int32_t	j;

	words = ft_strsplit(line, '-');
	if (words[0] && words[1] && !words[2])
	{
		i = (int)darr_flfind_i(farm->rooms, &words[0], ft_scmp);
		j = (int)darr_flfind_i(farm->rooms, &words[1], ft_scmp);
		state |= (i-- * j-- == 0) ? ERRSTATE : 0;
		if (i != j)
		{
			mtrx_set(farm, i, j);
			mtrx_set(farm, j, i);
		}
	}
	else
		state |= ERRSTATE;
	ft_del_arr((void ***)&words, 1);
	return (state);
}

void			form_rooms(t_source_farm *farm)
{
	size_t	i;
	char	*ptr[1];

	i = 0;
	while (i < darr_l(farm->rooms))
	{
		ptr[0] = darr(farm->names, *(size_t *)darr(farm->rooms, i));
		darr_eq(farm->rooms, i, ptr);
		i++;
	}
}

static int		read_room(int state, char *restrict line,
						t_source_farm *restrict farm)
{
	char **words;

	words = ft_strsplit(line, ' ');
	if (words[0] && !words[1])
	{
		mtrx_init(farm);
		darr_trim(farm->names);
		darr_trim(farm->rooms);
		form_rooms(farm);
		darr_sort(farm->rooms, ft_scmp, ft_qsort);
		state = (set_farm_se(farm)) ? (state | ERRSTATE) :
										(state & ~ROOMS) | TUBES;
	}
	else if (words[0] && words[1] && words[2] && !words[3])
	{
		if (ft_strchr(words[1], '-') || words[1][0] == 'L' ||
			*ft_skip_atoi(words[1]) || *ft_skip_atoi(words[2]))
			state |= ERRSTATE;
		darr_add(farm->rooms, ft_z(darr_l(farm->names)));
		g_start = (state & START) ? darr_l(farm->names) : g_start;
		g_end = (state & END) ? darr_l(farm->names) : g_end; //TODO: can del if
		darr_add_n(farm->names, words[0], ft_strlen(words[0]) + 1);
		state &= ~(START | END);
	}
	else
		state |= ERRSTATE;
	ft_del_arr((void ***)&words, 1);
	return (state);
}

static int		handle_line(char *restrict line, t_source_farm *restrict farm)
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
	return ((state & ERRSTATE));
}

int				handle_input(t_source_farm *restrict farm)
{
	int		ret;
	char	*line;

	while ((ret = ft_get_next_line(0, &line)))
	{
		if (ret < 0 || !line)
		{
			ft_memdel((void **)&line);
			return (1);
		}
		ret = handle_line(line, farm);
		ft_printf("%s\n", line);
		ft_free(line);
		if (ret)
			return (1);
	}
	ft_free(line);
	return (0);
}

//TODO: start = end?
