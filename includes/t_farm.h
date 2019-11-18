/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   t_farm.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtrisha <mtrisha@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/07 11:00:30 by mtrisha           #+#    #+#             */
/*   Updated: 2019/11/18 17:56:56 by mtrisha          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef T_FARM_H
# define T_FARM_H

# include <libft.h>

typedef struct	s_bit_connect_mtrx
{
	__uint64_t	*mtrx;
	__int32_t	mtrx_len;
}				t_bit_connect_mtrx;

typedef struct	s_source_farm
{
	size_t				ants;
	t_darr				rooms; //TODO: del mallocs???
	t_bit_connect_mtrx	bcmtrx;
	__int32_t			start;
	__int32_t			end;
}				t_source_farm;

# define MTRX(farm, i, j) (GETBIT_LLU((farm)->bcmtrx.mtrx[i * (farm) \
							->bcmtrx.mtrx_len + 1 + j / 64], j % 64))
# define MTRX_GETCON(farm, i) ((__int32_t)(farm)->bcmtrx.mtrx[ \
								i * (farm)->bcmtrx.mtrx_len])

void			src_farm_init(t_source_farm **farm);
void			src_farm_del(t_source_farm **farm);
void			mtrx_init(t_source_farm *farm);
void			mtrx_del(t_source_farm *farm);
int				mtrx(t_source_farm *farm, __int32_t i, __int32_t j);
void			mtrx_set(t_source_farm *farm, __int32_t i, __int32_t j);
void			mtrx_reset(t_source_farm *farm, __int32_t i, __int32_t j);
__int32_t		mtrx_getcon(t_source_farm *farm, __int32_t i);

typedef struct	s_mtrx_iterator
{
	__int32_t	row;
	__int32_t	i;
	__uint64_t	curitem;
	__int32_t	least;
}				t_mtrx_iter;

void			mtrx_iter_init(t_mtrx_iter *newiter,
								t_source_farm *farm, __int32_t i);
int				mtrx_next(t_mtrx_iter *iter, t_source_farm *farm);





typedef struct	s_graph_item
{
	size_t		id;
	__int32_t	con_count;
	__int32_t	state;
}				t_graph_item;

# define BASE_STATE 0

typedef struct	s_connect
{
	__int32_t	dst;
	__int32_t	weight;
}				t_connect;

typedef struct	s_graph
{
	void		*mem;
	__int32_t	size;
}				t_graph;

typedef struct	s_work_farm
{
	size_t		ants;
	t_darr		rooms;
	t_graph		graph;
	__int32_t	start;
	__int32_t	end;
}				t_work_farm;

# define GRAPH_ITEM(farm, i) (((t_graph_item **)(farm)->graph.mem)[i])
# define LIST_OF_CONNECTS(farm, i) ((t_connect *)(GRAPH_ITEM(farm, i) + 1))
# define GRAPH(farm, i, j) (connect_find(LIST_OF_CONNECTS(farm, i), \
						GRAPH_ITEM(farm, i)->con_count, j) ? 1 : 0);


void			work_farm_init(t_work_farm **farm, t_source_farm *src_farm);
void			work_farm_del(t_work_farm **farm);
t_graph_item	*graph_item(t_work_farm *farm, __int32_t i);
t_connect		*list_of_connects(t_work_farm *farm, __int32_t i);
int				graph(t_work_farm *farm, __int32_t i, __int32_t j);

t_connect		*connect_find(t_connect *connects, __int32_t count,
								__int32_t i);
void			graph_set(t_work_farm *farm, __int32_t i, t_connect connect);
void			graph_reset(t_work_farm *farm, __int32_t i, __int32_t j);

typedef struct	s_graph_iterator
{
	__int32_t	i;
	__int32_t	row;
}				t_graph_iter;

void			graph_iter_init(t_graph_iter *newiter, __int32_t i);
t_connect		*graph_next(t_graph_iter *iter, t_work_farm *farm);

#endif
