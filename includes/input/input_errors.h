/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_errors.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtrisha <mtrisha@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/09 15:36:20 by mtrisha           #+#    #+#             */
/*   Updated: 2019/12/10 17:34:29 by mtrisha          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef INPUT_ERRORS_H
# define INPUT_ERRORS_H

# include <string.h>

# define ANTS				(1 << 0)
# define ROOMS				(1 << 1)
# define TUBES				(1 << 2)
# define START				(1 << 3)
# define END				(1 << 4)

/*
**	NO_ERROR is a mask for delete or stay only error code in var
*/
# define NO_ERROR			((1 << 5) - 1)

# define ERRSTATE			(1 << 5)

# define WRONG_CMD			(1 << 6)
# define ANTS_ERROR			(1 << 7)
# define ROOM_ERROR			(1 << 8)
# define TOO_MUCH			(1 << 9)
# define GNL_ERROR			(1 << 10)
# define NO_UNIQ			(1 << 11)
# define TUBE_ERROR			(1 << 12)
# define SAME_WAYS			(1 << 13)
# define NO_ANTS			(1 << 14)
# define NO_ROOMS			(1 << 15)
# define NO_TUBES			(1 << 16)
# define NO_START_END		(1 << 17)
# define NO_POSSIBLE_WAY	(1 << 18)

# define ERRSTATE_MSG "Unknown error."
# define NO_POSSIBLE_WAY_MSG "There's no way between start and end!"
# define NO_START_END_MSG "There's no start/end room!"
# define NO_TUBES_MSG "There's no tubes!"
# define NO_ROOMS_MSG "There's no rooms!"
# define NO_ANTS_MSG "There's no ants!"
# define SAME_WAYS_MSG "Found several identical tubes."
# define TUBE_ERROR_MSG "Wrong tube definition."
# define NO_UNIQ_MSG "Found several rooms with identical names."
# define TOO_MUCH_MSG "Too much input rooms."
# define ROOM_ERROR_MSG "Wrong room definition."
# define ANTS_ERROR_MSG "Wrong ants definition."
# define WRONG_CMD_MSG "Wrong command lines."
# define GNL_ERROR_MSG "Reading input stream return error."

typedef struct	s_err
{
	__uint32_t	error_code;
	const char	*error_string;
}				t_err;

void			print_input_error(__uint32_t error);

#endif
