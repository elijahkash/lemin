/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_way_ants.h                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmathew <hmathew@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/10 14:46:00 by hmathew           #+#    #+#             */
/*   Updated: 2019/12/10 15:20:08 by hmathew          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PRINT_WAY_ANTS_H
# define PRINT_WAY_ANTS_H

# include "farm.h"
# include "libft.h"

/*
**	if (ant_name == -1) then room dont have ant
*/
typedef struct	s_print_room
{
	int			name;
	int 		ant_name;
}				t_print_room;

/*
**	need_go - the number of ants that must run along the way
*/
typedef struct	s_print_way
{
	t_print_room		*rooms;
	int			need_go;
}				t_print_way;


typedef struct	s_print_farm
{

}				t_print_farm;


void	print_result(t_enum_ways *restrict combs, t_farm *restrict farm);

#endif
