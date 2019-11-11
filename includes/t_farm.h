/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   t_farm.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtrisha <mtrisha@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/07 11:00:30 by mtrisha           #+#    #+#             */
/*   Updated: 2019/11/11 21:02:16 by mtrisha          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef T_FARM_H
# define T_FARM_H

# include <libft.h>

typedef struct	s_farm
{
	int			ants;
	t_darr		rooms;
	size_t		size;
	__uint64_t	*mtrx;
	int			mtrx_len;
	int			start;
	int			end;
}				t_farm;

void			farm_init(t_farm *farm);
void			mtrx_init(t_farm *farm);
int				mtrx(t_farm farm, int i, int j);
void			mtrx_set(t_farm farm, int i, int j);
int				mtrx_getcon(t_farm farm, int i);

typedef struct	s_iterator
{
	int			row;
	int			i;
	__uint64_t	curitem;
	int			least;
}				t_iter;

void			iter_init(t_iter *newiter, t_farm farm, int i);
int				next(t_iter *iter, t_farm farm);

#endif
