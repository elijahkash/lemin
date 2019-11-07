/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   t_farm.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtrisha <mtrisha@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/07 11:00:30 by mtrisha           #+#    #+#             */
/*   Updated: 2019/11/07 12:46:05 by mtrisha          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef T_FARM_H
# define T_FARM_H

# include <libft.h>

typedef struct	s_farm
{
	int		ants;
	t_darr	rooms;
	char	*mtrx;
	int		start;
	int		end;
	int		way_count;
}				t_farm;
//TODO: max - int rooms

void			farm_init(t_farm *farm);
void			mtrx_init(t_farm *farm);
char			mtrx(t_farm farm, int i, int j);

#endif
