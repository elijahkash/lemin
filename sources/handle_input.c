/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_input.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtrisha <mtrisha@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/07 12:56:25 by mtrisha           #+#    #+#             */
/*   Updated: 2019/11/13 20:44:25 by mtrisha          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <handle_input.h>

#include <limits.h>

static char	*g_start = NULL;
static char *g_end = NULL;

static int		set_farm_se(t_farm *restrict farm)
{
	register size_t	i;
	register size_t size;

	size = (int)*farm->rooms.curlen;
	i = 0;
	while (i < size)
	{
		farm->start = (*(char **)darr(farm->rooms, i) == g_start) ?
						i : farm->start;
		farm->end = (*(char **)darr(farm->rooms, i) == g_end) ?
						i : farm->end;
		if (i < size - 1 && !ft_strcmp(*(char **)darr(
			farm->rooms, i), *(char **)darr(farm->rooms, i + 1)))
			return (1);
		i++;
	}
	return (0);
}

static int		handle_cmd(int state, char *restrict line,
							t_farm *restrict farm)
{
	if (!ft_strcmp(line, "##start"))
		state |= (state & (START | END) || farm->start != -1) ? ERRSTATE :
																START;
	else if (!ft_strcmp(line, "##end"))
		state |= (state & (START | END) || farm->end != -1) ? ERRSTATE : END;
	return (state + (int)farm * 0);
}

static int		read_ants(int state, char *restrict line, t_farm *restrict farm)
{
	if (state & (START | END) || ft_isdigit_ws(line))
		return (state | ERRSTATE);
	farm->ants = ft_atoi(line);
	state &= ~ANTS;
	state |= ROOMS;
	return (state);
}

static int		read_tube(int state, char *restrict line, t_farm *restrict farm)
{
	char	**words;
	int		i;
	int		j;

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

static int		read_room(int state, char *restrict line, t_farm *restrict farm)
{
	char **words;

	words = ft_strsplit(line, ' ');
	if (words[0] && !words[1])
	{
		mtrx_init(farm);
		darr_sort(farm->rooms, ft_scmp, ft_qsort);
		state = (set_farm_se(farm)) ? (state | ERRSTATE) :
										(state & ~ROOMS) | TUBES;
	}
	else if (words[0] && words[1] && words[2] && !words[3])
	{
		if (ft_strchr(words[1], '-') || words[1][0] == 'L' ||
			*ft_skip_atoi(words[1]) || *ft_skip_atoi(words[2]))
			state |= ERRSTATE;
		darr_add_str(farm->rooms, words[0]);
		g_start = (state & START) ? *(char **)darr_top(farm->rooms) : g_start;
		g_end = (state & END) ? *(char **)darr_top(farm->rooms) : g_end;
		state &= ~(START | END);
	}
	else
		state |= ERRSTATE;
	ft_del_arr((void ***)&words, 1);
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
	return ((state & ERRSTATE));
}

int				handle_input(t_farm *restrict farm)
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
