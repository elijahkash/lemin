/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_errors.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtrisha <mtrisha@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/10 17:27:59 by mtrisha           #+#    #+#             */
/*   Updated: 2019/12/10 17:33:03 by mtrisha          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <input_errors.h>

#include <libft.h>

static const t_err	g_errlist[] =
{
	{ERRSTATE, ERRSTATE_MSG},
	{NO_POSSIBLE_WAY, NO_POSSIBLE_WAY_MSG},
	{NO_START_END, NO_START_END_MSG},
	{NO_TUBES, NO_TUBES_MSG},
	{NO_ROOMS, NO_ROOMS_MSG},
	{NO_ANTS, NO_ANTS_MSG},
	{SAME_WAYS, SAME_WAYS_MSG},
	{TUBE_ERROR, TUBE_ERROR_MSG},
	{NO_UNIQ, NO_UNIQ_MSG},
	{TOO_MUCH, TOO_MUCH_MSG},
	{ROOM_ERROR, ROOM_ERROR_MSG},
	{ANTS_ERROR, ANTS_ERROR_MSG},
	{WRONG_CMD, WRONG_CMD_MSG},
	{GNL_ERROR, GNL_ERROR_MSG}
};

void				print_input_error(__uint32_t error)
{
	const char		*err_str;
	__uint32_t 		i;

	i = (sizeof(g_errlist) / sizeof(t_err));
	while (i-- > 0 )
		if (g_errlist[i].error_code & error)
		{
			err_str = g_errlist[i].error_string;
			break ;
		}
	ft_printf("ERROR: \"%s\"\n", err_str);
}
