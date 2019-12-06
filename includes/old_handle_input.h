/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   old_handle_input.h                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtrisha <mtrisha@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/07 12:55:57 by mtrisha           #+#    #+#             */
/*   Updated: 2019/12/06 11:23:28 by mtrisha          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HANDLE_INPUT_H
# define HANDLE_INPUT_H

# include <old_farm.h>

# define ANTS 1
# define ROOMS 2
# define TUBES 4
# define START 8
# define END 16
# define ERRSTATE 1024

int		handle_input(t_source_farm *farm);

#endif
