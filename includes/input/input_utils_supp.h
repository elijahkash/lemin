/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_utils_supp.h                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Kashnitskiy <elijahkash.code@gmail.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/09 14:24:17 by mtrisha           #+#    #+#             */
/*   Updated: 2020/01/27 17:06:40 by Kashnitskiy      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef INPUT_UTILS_SUPP_H
# define INPUT_UTILS_SUPP_H

# include <farm.h>

t_uint	is_uniq_names(t_vect *names);
void	form_rooms(t_farm *farm);
t_uint	is_room(char *line);

#endif
