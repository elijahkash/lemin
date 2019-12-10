/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_errors.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtrisha <mtrisha@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/09 15:36:20 by mtrisha           #+#    #+#             */
/*   Updated: 2019/12/10 13:25:30 by mtrisha          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef INPUT_ERRORS_H
# define INPUT_ERRORS_H

# define ANTS				(1 << 1)
# define ROOMS				(1 << 2)
# define TUBES				(1 << 3)
# define START				(1 << 4)
# define END				(1 << 5)

/*
**	NO_ERROR is a mask for delete or stay only error code in var
*/
# define NO_ERROR			((1 << 6) - 1)

# define ERRSTATE			(1 << 6)

# define WRONG_CMD			(1 << 7)
# define ANTS_ERROR			(1 << 8)
# define ROOM_ERROR			(1 << 9)
# define TOO_MUCH			(1 << 10)
# define GNL_ERROR			(1 << 11)
# define NO_UNIQ			(1 << 12)
# define TUBE_ERROR			(1 << 13)
# define SAME_WAYS			(1 << 14)
# define NO_ANTS			(1 << 15)
# define NO_ROOMS			(1 << 16)
# define NO_TUBES			(1 << 17)
# define NO_START_END		(1 << 18)
# define NO_POSSIBLE_WAY	(1 << 19)

typedef struct	s_err
{
	__uint32_t	error_code;
	const char	*error_string;
}				t_err;

#endif
