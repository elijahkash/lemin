/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_utils_basic.h                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Kashnitskiy <elijahkash.code@gmail.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/09 12:55:41 by mtrisha           #+#    #+#             */
/*   Updated: 2020/01/27 17:06:41 by Kashnitskiy      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef INPUT_UTILS_BASIC_H
# define INPUT_UTILS_BASIC_H

# include <farm.h>

t_uint	read_tube(t_uint state, char *line, t_farm *farm);
t_uint	read_room(t_uint state, char *line, t_farm *farm);
t_uint	read_ants(t_uint state, char *line, t_farm *farm);
t_uint	handle_cmd(t_uint state, char *line, t_farm *farm);

#endif
