/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_input.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtrisha <mtrisha@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/07 12:56:25 by mtrisha           #+#    #+#             */
/*   Updated: 2019/11/07 20:20:03 by mtrisha          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <handle_input.h>

#include <limits.h>

static char	*g_start = NULL;
static char *g_end = NULL;

static int		set_farm_se(t_farm *farm)
{
	size_t	i;

	i = 0;
	while (i < *farm->size)
	{
		if (*(char **)darr(farm->rooms, i) == g_start)
			farm->start = i;
		if (*(char **)darr(farm->rooms, i) == g_end)
			farm->end = i;
		if (i < *farm->size - 1 && !ft_strcmp(*(char **)darr(farm->rooms, i),
											*(char **)darr(farm->rooms, i + 1)))
			return (1);
		i++;
	}
	return (0);
}

static int		handle_cmd(int state, char *line, t_farm *farm)
{
	if (!ft_strcmp(line, "##start"))
		state |= (state & (START | END) || farm->start != -1) ? ERRSTATE :
																START;
	else if (!ft_strcmp(line, "##end"))
		state |= (state & (START | END) || farm->end != -1) ? ERRSTATE : END;
	return (state + (int)farm * 0);
}

static int		read_ants(int state, char *line, t_farm *farm)
{
	if (state & (START | END) || ft_isdigit_ws(line))
		return (state | ERRSTATE);
	farm->ants = ft_atoi(line);
	state &= ~ANTS;
	state |= ROOMS;
	return (state);
}

static int		read_tube(int state, char *line, t_farm *farm)
{
	char	**words;
	int		i;
	int		j;

	words = ft_strsplit(line, '-');
	if (words[0] && words[1] && !words[2])
	{
		i = (int)darr_flfind_i(farm->rooms, &words[0], ft_scmp);
		j = (int)darr_flfind_i(farm->rooms, &words[1], ft_scmp);
		if (i * j == 0)
			state |= ERRSTATE;
		mtrx_set(*farm, i - 1, j - 1, 1);
	}
	else
		state |= ERRSTATE;
	ft_del_arr((void ***)&words, 1);
	return (state);
}

static int		read_room(int state, char *line, t_farm *farm)
{
	char **words;

	words = ft_strsplit(line, ' ');
	if (words[0] && !words[1])
	{
		ft_free(*words);
		ft_free(words);
		mtrx_init(farm);
		darr_sort(farm->rooms, ft_scmp, ft_qsort);
		if (set_farm_se(farm))
			return (state | ERRSTATE);
		return(read_tube((state & ~ROOMS) | TUBES, line, farm));
	}
	if (*farm->size == INT_MAX)
		state |= ERRSTATE;
	else if (words[0] && words[1] && words[2] && !words[3])
	{
		if (ft_strchr(words[1], '-') || words[1][0] == 'L' ||
			*ft_skip_atoi(words[1]) || *ft_skip_atoi(words[2]))
			state |= ERRSTATE;
		else
		{
			darr_add_str(farm->rooms, words[0]);
			if (state & START)
				g_start = *(char **)darr_top(farm->rooms);
			if (state & END)
				g_end = *(char **)darr_top(farm->rooms);
			state &= ~(START | END);
		}
	}
	else
		state |= ERRSTATE;
	ft_del_arr((void ***)&words, 1);
	return (state);
}

static int		handle_line(char *line, t_farm *farm)
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
	else if (state & TUBES)
		state = read_tube(state, line, farm);
	else
		ft_printf("SHIT\n");
	if (state & ERRSTATE)
		return (1);
	return (0);
}

int				handle_input(t_farm *farm)
{
	int		ret;
	char	*line;

	while ((ret = ft_get_next_line(0, &line)))
	{
		if (ret < 0 || !line)
		{
			if (line)
				ft_free(line);
			return (1);
		}
		ret = handle_line(line, farm);
		ft_printf("%s\n", line);
		ft_free(line);
		if (ret)
			return (1);
	}//TODO way_count
	ft_free(line);
	return (0);
}
