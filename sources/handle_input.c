/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_input.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtrisha <mtrisha@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/07 12:56:25 by mtrisha           #+#    #+#             */
/*   Updated: 2019/11/07 14:56:42 by mtrisha          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <handle_input.h>

static int handle_line(char *line, t_farm *farm)
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
}

int			handle_input(t_farm *farm)
{
	int		ret;
	char	line;

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
	}
	ft_free(line);
	return (0);
}
