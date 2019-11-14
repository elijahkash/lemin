/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   t_farm.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtrisha <mtrisha@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/07 11:00:30 by mtrisha           #+#    #+#             */
/*   Updated: 2019/11/14 18:21:31 by mtrisha          ###   ########.fr       */
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

# define GRAPH_ITEM(i) (*((t_graph_item **)farm->work_graph.mem)[i])

typedef struct	s_connect
{
	int		dst;
	int		weight;
}				t_connect;

typedef struct	s_work_graph
{
	void	*mem;
	int		size;
}				t_work_graph;

typedef struct	s_farm
{
	int					ants;
	t_darr				rooms;
	t_bit_connect_mtrx	bcmtrx;
	t_work_graph		work_graph;
	int					start;
	int					end;
}				t_farm;

void			farm_init(t_farm *farm);
void			mtrx_init(t_farm *farm);
int				mtrx(t_farm *farm, int i, int j);
void			mtrx_set(t_farm *farm, int i, int j);
void			mtrx_reset(t_farm *farm, int i, int j);
int				mtrx_getcon(t_farm *farm, int i);

typedef struct	s_mtrx_iterator
{
	int			row;
	int			i;
	__uint64_t	curitem;
	int			least;
}				t_mtrx_iter;

void			mtrx_iter_init(t_mtrx_iter *newiter, t_farm *farm, int i);
int				mtrx_next(t_mtrx_iter *iter, t_farm *farm);

typedef struct	s_graph_iterator
{
	int			i;
	int			row;
}				t_iter;

void			iter_init(t_iter *newiter, int i);
t_connect		*next(t_iter *iter, t_farm *farm);

#endif
