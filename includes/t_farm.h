/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   t_farm.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtrisha <mtrisha@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/07 11:00:30 by mtrisha           #+#    #+#             */
/*   Updated: 2019/11/13 20:44:29 by mtrisha          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef T_FARM_H
# define T_FARM_H

# include <libft.h>

typedef struct	s_bit_connect_mtrx
{
	__uint64_t	*mtrx;
	int			mtrx_len;
}				t_bit_connect_mtrx;

typedef struct	s_graph_item
{
	int		id;
	int		con_count;
	int		state;
}				t_graph_item;

typedef struct	s_farm
{
	int					ants;
	t_darr				rooms;
	t_bit_connect_mtrx	bcmtrx;
	void				*work_graph;
	int					start;
	int					end;
}				t_farm;

void			farm_init(t_farm *restrict farm);
void			mtrx_init(t_farm *restrict farm);
int				mtrx(t_farm *restrict farm, int i, int j);
void			mtrx_set(t_farm *restrict farm, int i, int j);
void			mtrx_reset(t_farm *restrict farm, int i, int j);
int				mtrx_getcon(t_farm *restrict farm, int i);

typedef struct	s_iterator
{
	int			row;
	int			i;
	__uint64_t	curitem;
	int			least;
}				t_iter;

void			iter_init(t_iter *restrict newiter, t_farm *restrict farm,
							register int i);
int				next(t_iter *restrict iter, t_farm *restrict farm);

#endif
