/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   t_farm.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtrisha <mtrisha@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/07 11:00:30 by mtrisha           #+#    #+#             */
/*   Updated: 2019/11/16 22:13:55 by mtrisha          ###   ########.fr       */
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

typedef struct	s_source_farm
{
	int					ants;
	t_darr				rooms;
	t_bit_connect_mtrx	bcmtrx;
	int					start;
	int					end;
}				t_source_farm;

# define MTRX(farm, i, j) (GETBIT_LLU((farm)->bcmtrx.mtrx[i * (farm) \
							->bcmtrx.mtrx_len + 1 + j / 64], j % 64))
# define MTRX_GETCON(farm, i) ((int)(farm)->bcmtrx.mtrx[ \
								i * (farm)->bcmtrx.mtrx_len])

void			farm_init(t_source_farm **farm);
void			farm_del(t_source_farm **farm);
void			mtrx_init(t_source_farm *farm);
void			mtrx_del(t_source_farm *farm);
int				mtrx(t_source_farm *farm, int i, int j);
void			mtrx_set(t_source_farm *farm, int i, int j);
void			mtrx_reset(t_source_farm *farm, int i, int j);
int				mtrx_getcon(t_source_farm *farm, int i);

typedef struct	s_mtrx_iterator
{
	int			row;
	int			i;
	__uint64_t	curitem;
	int			least;
}				t_mtrx_iter;

void			mtrx_iter_init(t_mtrx_iter *newiter,
								t_source_farm *farm, int i);
int				mtrx_next(t_mtrx_iter *iter, t_source_farm *farm);





typedef struct	s_graph_item
{
	size_t	id;
	int		con_count;
	int		state;
}				t_graph_item;

typedef struct	s_connect
{
	int		dst;
	int		weight;
}				t_connect;

typedef struct	s_graph
{
	void	*mem;
	int		size;
}				t_graph;

typedef struct	s_work_farm
{
	int					ants;
	t_darr				rooms;
	t_graph				graph;
	int					start;
	int					end;
}				t_work_farm;

# define GRAPH_ITEM(farm, i) (((t_graph_item **)(farm)->graph.mem)[i])
# define LIST_OF_CONNECTS(farm, i) ((t_connect *)(GRAPH_ITEM(farm, i) + 1))
# define GRAPH(farm, i, j) (connect_find(LIST_OF_CONNECTS(farm, i), \
						GRAPH_ITEM(farm, i)->con_count, j) ? 1 : 0);

t_graph_item	*graph_item(t_work_farm *farm, int i);
t_connect		*list_of_connects(t_work_farm *farm, int i);
int				graph(t_work_farm *farm, int i, int j);

t_connect		*connect_find(t_connect *connects, int count, int i);

typedef struct	s_graph_iterator
{
	int			i;
	int			row;
}				t_graph_iter;

void			graph_iter_init(t_graph_iter *newiter, int i);
t_connect		*graph_next(t_graph_iter *iter, t_work_farm *farm);

#endif
