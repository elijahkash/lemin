/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_input.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtrisha <mtrisha@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/09 12:54:24 by mtrisha           #+#    #+#             */
/*   Updated: 2019/12/09 15:00:46 by mtrisha          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef READ_INPUT_H
# define READ_INPUT_H

# include <farm.h>

# define ANTS		1
# define ROOMS		2
# define TUBES		4
# define START		8
# define END		16

# define ERRSTATE	32
# define WRONG_CMD	64
# define ANTS_ERROR	128
# define ROOM_ERROR	256
# define TOO_MUCH	512
# define GNL_ERROR	1024
# define NO_UNIQ	2048
# define TUBE_ERROR	4096

# define NO_ERROR	31

int		read_input(t_farm *restrict farm);

#endif
