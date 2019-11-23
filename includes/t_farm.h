/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   t_farm.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtrisha <mtrisha@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/07 11:00:30 by mtrisha           #+#    #+#             */
/*   Updated: 2019/11/23 18:13:53 by mtrisha          ###   ########.fr       */
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
	t_darr				names;
	t_darr				rooms;
	t_bit_connect_mtrx	bcmtrx;
	__int32_t			start;
	__int32_t			end;
}				t_source_farm;

# define INIT_COUNT_ROOMS 128
# define INIT_COUNT_NAMES 1024

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
	__int32_t	weight; // really need?????
	__int32_t	state; // char?
}				t_graph_item;

# define INIT_WEIGHT 0
# define INIT_STATE 0
# define BASE_STATE 0 // 24; need safe separete when clean graph!
# define MARKED		1
# define MARKED_IN	2
# define MARKED_OUT 4
# define SEPARATE	8

//TODO: api for fast get lst bit?
// +1 !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
typedef struct	s_connect
{
	__int32_t	dst;
	// __int32_t	weight;
	char		state;
}				t_connect;

# define WAY_BASE_STATE 1
# define WAY_NEGATIVE	2
# define WAY_FORBIDDEN	4
// # define END_WAY	3

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
t_connect		*graph_connect(t_work_farm *farm, __int32_t i, __int32_t j);
int				graph_state(t_work_farm *farm, __int32_t i, __int32_t j);

t_connect		*connect_find(t_connect *connects, __int32_t count,
								__int32_t i);
void			graph_set(t_work_farm *farm, __int32_t i, t_connect connect);
void			graph_reset(t_work_farm *farm, __int32_t i, __int32_t j);

typedef struct	s_graph_iterator
{
	__int32_t	i;
	__int32_t	row;
	char		state; //TODO: can del and use row bit ???
}				t_graph_iter;

# define ALL_WAYS	0
# define ALLOW_WAYS	1
# define NEG_WAYS	2

void			graph_iter_init(t_graph_iter *newiter, __int32_t i,
								t_work_farm *farm);
t_connect		*graph_next(t_graph_iter *iter, t_work_farm *farm);
t_connect		*graph_next_all(t_graph_iter *iter, t_work_farm *farm);
t_connect		*graph_next_allow(t_graph_iter *iter, t_work_farm *farm);
t_connect		*graph_next_neg(t_graph_iter *iter, t_work_farm *farm);

extern t_connect		*(*iter_func[])(t_graph_iter *iter, t_work_farm *farm);





void			clean_graph_state(t_work_farm *farm);






typedef struct	s_full_connect
{
	__int32_t	src;
	__int32_t	dst;
}				t_full_connect;




#endif
