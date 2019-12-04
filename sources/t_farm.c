/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   t_farm.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtrisha <mtrisha@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/07 11:01:17 by mtrisha           #+#    #+#             */
/*   Updated: 2019/12/03 19:55:26 by mtrisha          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <t_farm.h>

#include <prepare_work_graph.h> //FIXME: ????

void		src_farm_init(t_source_farm **farm)
{
	*farm = ft_malloc(sizeof(t_source_farm));
	(*farm)->ants = 0;
	(*farm)->end = -1;
	(*farm)->start = -1;
	(*farm)->bcmtrx.mtrx_len = 0;
	(*farm)->bcmtrx.mtrx = NULL;
	darr_init(&((*farm)->rooms), PTR_SIZE, INIT_COUNT_ROOMS);
	darr_init(&((*farm)->names), sizeof(char), INIT_COUNT_NAMES);
}

void		src_farm_del(t_source_farm **farm)
{
	mtrx_del(*farm);
	darr_del(&((*farm)->rooms));
	darr_del(&((*farm)->names));
	ft_free(*farm);
}

int			mtrx(t_source_farm *farm, __int32_t i, __int32_t j)
{
	return (GETBIT_LLU(
		farm->bcmtrx.mtrx[i * farm->bcmtrx.mtrx_len + 1 + j / 64], j % 64));
}

__int32_t	mtrx_getcon(t_source_farm *farm, __int32_t i)
{
	return ((__int32_t)farm->bcmtrx.mtrx[i * farm->bcmtrx.mtrx_len]);
}

void		mtrx_set(t_source_farm *farm, __int32_t i, __int32_t j)
{
	register __int32_t	pos;
	register __int32_t	bit;

	pos = i * farm->bcmtrx.mtrx_len + 1 + j / 64;
	bit = j % 64;
	farm->bcmtrx.mtrx[i * farm->bcmtrx.mtrx_len] +=
		(!GETBIT_LLU(farm->bcmtrx.mtrx[pos], bit));
	farm->bcmtrx.mtrx[pos] = SETBIT_LLU(farm->bcmtrx.mtrx[pos], bit);
	return ;
}

void		mtrx_reset(t_source_farm *farm, __int32_t i, __int32_t j)
{
	register __int32_t	pos;
	register __int32_t	bit;

	pos = i * farm->bcmtrx.mtrx_len + 1 + j / 64;
	bit = j % 64;
	farm->bcmtrx.mtrx[i * farm->bcmtrx.mtrx_len] -=
		(GETBIT_LLU(farm->bcmtrx.mtrx[pos], bit));
	farm->bcmtrx.mtrx[pos] = RESETBIT_LLU(farm->bcmtrx.mtrx[pos], bit);
	return ;
}

void		mtrx_init(t_source_farm *farm)
{
	register __int32_t size;

	size = (__int32_t)*farm->rooms.curlen;
	farm->bcmtrx.mtrx_len = (size / 64 + ((size % 64) ? 2 : 1));
	farm->bcmtrx.mtrx = (__uint64_t *)ft_memalloc(8 * size *
												farm->bcmtrx.mtrx_len);
}

void		mtrx_del(t_source_farm *farm)
{
	ft_memdel((void **)&(farm->bcmtrx.mtrx));
	farm->bcmtrx.mtrx_len = 0;
}

void		mtrx_iter_init(t_mtrx_iter *newiter, t_source_farm *farm,
							__int32_t i)
{
	newiter->row = i;
	newiter->i = 1;
	newiter->curitem = farm->bcmtrx.mtrx[i * farm->bcmtrx.mtrx_len + 1];
	newiter->least = farm->bcmtrx.mtrx[i * farm->bcmtrx.mtrx_len];
}

int			mtrx_next(t_mtrx_iter *iter, t_source_farm *farm)
{
	register __uint64_t tmp;

	if (!iter->least)
		return (-1);
	while (!iter->curitem)
		iter->curitem = farm->bcmtrx.mtrx[iter->row * farm->bcmtrx.mtrx_len +
											++(iter->i)];
	iter->least--;
	tmp = iter->curitem;
	BSR_ASM(tmp);
	iter->curitem = RESETBIT_LLU(iter->curitem, tmp);
	return (64 * (iter->i - 1) + (int)tmp);
}








void			work_farm_init(t_work_farm **farm, t_source_farm *src_farm)
{
	*farm = ft_malloc(sizeof(t_work_farm));
	(*farm)->ants = src_farm->ants;
	(*farm)->graph.mem = NULL;
	create_work_farm(*farm, src_farm);
}

void			work_farm_del(t_work_farm **farm)
{
	darr_del(&((*farm)->rooms));
	ft_free((*farm)->graph.mem);
	ft_free(*farm);
}

t_graph_item	*graph_item(t_work_farm *farm, __int32_t i)
{
	return (((t_graph_item **)farm->graph.mem)[i]);
}

t_connect		*list_of_connects(t_work_farm *farm, __int32_t i)
{
	return (((t_connect *)&(((t_graph_item **)farm->graph.mem)[i][1])));
}

int				graph(t_work_farm *farm, __int32_t i, __int32_t j)
{
	return (connect_find(list_of_connects(farm, i),
						graph_item(farm, i)->con_count, j) ? 1 : 0);
}

t_connect		*graph_connect(t_work_farm *farm, __int32_t i, __int32_t j)
{
	return (connect_find(list_of_connects(farm, i),
						graph_item(farm, i)->con_count, j));
}

int				graph_state(t_work_farm *farm, __int32_t i, __int32_t j)
{
	t_connect	*tmp;

	tmp = connect_find(list_of_connects(farm, i),
						graph_item(farm, i)->con_count, j);
	return (tmp ? tmp->state : 0);
}

t_connect		*connect_find(t_connect *connects, __int32_t count, __int32_t i)
{
	__int32_t		bot;
	__int32_t		top;
	__int32_t		tmp;

	if (!count)
		return (NULL);
	top = count - 1;
	bot = 0;
	while (bot != top)
	{
		tmp = i - connects[bot + (top - bot) / 2].dst;
		if (!tmp)
			return (connects + bot + (top - bot) / 2);
		if (tmp < 0)
			top = bot + (top - bot) / 2;
		else
			bot += (top - bot) / 2 + 1;
	}
	tmp = i - connects[bot].dst;
	return (tmp ? NULL : connects + bot);
}

void		graph_set(t_work_farm *farm, __int32_t i, t_connect connect)
{
	__int32_t			bot;
	__int32_t			top;
	__int32_t			tmp; // can be deleted (norminette)
	t_connect	*ptr;

	ptr = list_of_connects(farm, i);
	if (!GRAPH_ITEM(farm, i)->con_count)
	{
		ptr[0] = connect;
		return ;
	}
	top = GRAPH_ITEM(farm, i)->con_count - 1;
	bot = 0;
	while (bot != top)
	{
		tmp = connect.dst - ptr[bot + (top - bot) / 2].dst;
		if (tmp < 0)
			top = bot + (top - bot) / 2;
		else
			bot += (top - bot) / 2 + 1;
	}
	tmp = connect.dst - ptr[bot].dst;
	if (!tmp)
		return ;
	bot += (tmp > 0) ? 1 : 0;
	//FIXME: here ?
	ft_memmove(ptr + bot + 1, ptr + bot,
		sizeof(t_connect) * (GRAPH_ITEM(farm, i)->con_count++ - bot));
	ptr[bot] = connect;
	return ;
}

void		graph_reset(t_work_farm *farm, __int32_t i, __int32_t j)
{
	__int32_t			bot;
	__int32_t			top;
	__int32_t			tmp;
	t_connect	*ptr;

	ptr = list_of_connects(farm, i);
	if (!GRAPH_ITEM(farm, i)->con_count)
		return ;
	top = GRAPH_ITEM(farm, i)->con_count - 1;
	bot = 0;
	while (bot != top)
	{
		tmp = j - ptr[bot + (top - bot) / 2].dst;
		if (!tmp)
		{
			bot += (top - bot) / 2;
			break ;
		}
		if (tmp < 0)
			top = bot + (top - bot) / 2;
		else
			bot += (top - bot) / 2 + 1;
	}
	tmp = j - ptr[bot].dst;
	if (tmp)
		return ;
	ft_memmove(ptr + bot, ptr + bot + 1,
		sizeof(t_connect) * (GRAPH_ITEM(farm, i)->con_count - bot));
	GRAPH_ITEM(farm, i)->con_count--;
	return ;
}

void			graph_iter_init(t_graph_iter *newiter, __int32_t i,
								t_work_farm *farm)
{
	newiter->i = 0;
	newiter->row = i;
	if (!farm) // forb ways!!!!!!!!
		newiter->state = ALL_WAYS;
	else
		newiter->state = (GRAPH_ITEM(farm, i)->state & MARKED &&
						GRAPH_ITEM(farm, i)->state & MARKED_IN) ?
							NEG_WAYS : ALLOW_WAYS;
}

t_connect		*graph_next(t_graph_iter *iter, t_work_farm *farm)
{
	return (iter_func[(int)iter->state](iter, farm)); // int?
}

t_connect		*graph_next_all(t_graph_iter *iter, t_work_farm *farm)
{
	return ((iter->i < GRAPH_ITEM(farm, iter->row)->con_count) ?
	&(LIST_OF_CONNECTS(farm, iter->row)[(iter->i)++]) : NULL);
}

t_connect		*graph_next_allow(t_graph_iter *iter, t_work_farm *farm)
{
	register t_connect	*tmp;

	while ((tmp = graph_next_all(iter, farm)) && (tmp->state & WAY_FORBIDDEN))
		continue;
	return (tmp);
}

t_connect		*graph_next_neg(t_graph_iter *iter, t_work_farm *farm)
{
	register t_connect	*tmp;

	while ((tmp = graph_next_all(iter, farm)) && ((tmp->state & WAY_FORBIDDEN)
			|| !(tmp->state & WAY_NEGATIVE))) //????
		continue;
	return (tmp);
}

t_connect		*graph_next_forb(t_graph_iter *iter, t_work_farm *farm)
{
	register t_connect	*tmp;

	while ((tmp = graph_next_all(iter, farm)) && !(tmp->state & WAY_FORBIDDEN))
		continue;
	return (tmp);
}

t_connect		*(*iter_func[])(t_graph_iter *iter, t_work_farm *farm) = {
	graph_next_all,
	graph_next_allow,
	graph_next_neg,
	graph_next_forb
};










void			clean_graph_state(t_work_farm *farm)
{
	__int32_t	i;

	i = 0;
	while (i < farm->graph.size)
	{
		GRAPH_ITEM(farm, i)->state &= SEPARATE; // !!!!!
		GRAPH_ITEM(farm, i)->weight = INIT_WEIGHT; // really need?
		i++;
	}
}

