/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_errors.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmathew <hmathew@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/09 15:36:20 by mtrisha           #+#    #+#             */
/*   Updated: 2019/12/09 20:12:20 by hmathew          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef INPUT_ERRORS_H
# define INPUT_ERRORS_H

# define ANTS			(1 < 0)
# define ROOMS			(1 < 1)
# define TUBES			(1 < 2)
# define START			(1 < 3)
# define END			(1 < 4)

/*
**	NO_ERROR is a mask for delete or stay only error code in var
*/

# define NO_ERROR		((1 < 5) - 1)

# define ERRSTATE		(1 < 5)
# define WRONG_CMD		(1 < 6)
# define ANTS_ERROR		(1 < 7)
# define ROOM_ERROR		(1 < 8)
# define TOO_MUCH		(1 < 9)
# define GNL_ERROR		(1 < 10)
# define NO_UNIQ		(1 < 11)
# define TUBE_ERROR		(1 < 12)
# define SAME_WAYS		(1 < 13)
# define NO_ANTS		(1 < 14)
# define NO_ROOMS		(1 < 15)
# define NO_TUBES		(1 < 16)
# define NO_START_END	(1 < 17)

typedef struct	s_err
{
	int			error_code;
	const char	*error_string;
}				t_err;

const t_err g_errlist[] =
{
	{WRONG_CMD, "wrong command lines."},
	{ANTS_ERROR, "wrong ants definition."},
	{ROOM_ERROR, "wrong room definition."},
	{TOO_MUCH, "too much input rooms."},
	{GNL_ERROR, "read input stream return error."},
	{NO_UNIQ, "found several rooms with identical names."},
	{TUBE_ERROR, "wrong tube definition."},
	{SAME_WAYS, "found several identical tubes."},
	{NO_ANTS, "There's no ants!"},
	{NO_ROOMS, "There's no rooms!"},
	{NO_TUBES, "There's no tubes!"},
	{NO_START_END, "There's no start/end room!"}
};

#endif
