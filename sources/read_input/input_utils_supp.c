/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_utils_supp.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Kashnitskiy <elijahkash.code@gmail.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/09 12:59:14 by mtrisha           #+#    #+#             */
/*   Updated: 2020/01/27 17:07:07 by Kashnitskiy      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <input_utils_supp.h>

t_uint		is_uniq_names(t_vect *restrict names)
{
	size_t	i;
	size_t	is_error;
	size_t	size;

	is_error = 0;
	if (names->curlen == 0)
		return (1);
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

void		form_rooms(t_farm *restrict farm)
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

t_uint		is_room(char *restrict line)
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
