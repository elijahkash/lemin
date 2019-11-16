/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   t_farm.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtrisha <mtrisha@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/07 11:01:17 by mtrisha           #+#    #+#             */
/*   Updated: 2019/11/16 22:58:26 by mtrisha          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <t_farm.h>

void		farm_init(t_source_farm **farm)
{
	*farm = ft_malloc(sizeof(t_source_farm));
	(*farm)->ants = 0;
	(*farm)->end = -1;
	(*farm)->start = -1;
	(*farm)->bcmtrx.mtrx_len = 0;
	(*farm)->bcmtrx.mtrx = NULL;
	// farm->work_graph.mem = NULL;
	// farm->work_graph.size = 0;
	darr_init(&((*farm)->rooms), PTR_SIZE, 128);
}

void		farm_del(t_source_farm **farm)
{
	mtrx_del(*farm);
	darr_del(&((*farm)->rooms));
	ft_free(*farm);
}

int			mtrx(t_source_farm *farm, int i, int j)
{
	return (GETBIT_LLU(
		farm->bcmtrx.mtrx[i * farm->bcmtrx.mtrx_len + 1 + j / 64], j % 64));
}

int			mtrx_getcon(t_source_farm *farm, int i)
{
	return ((int)farm->bcmtrx.mtrx[i * farm->bcmtrx.mtrx_len]);
}

void		mtrx_set(t_source_farm *farm, int i, int j)
{
	register int	pos;
	register int	bit;

	pos = i * farm->bcmtrx.mtrx_len + 1 + j / 64;
	bit = j % 64;
	farm->bcmtrx.mtrx[i * farm->bcmtrx.mtrx_len] +=
		(!GETBIT_LLU(farm->bcmtrx.mtrx[pos], bit));
	farm->bcmtrx.mtrx[pos] = SETBIT_LLU(farm->bcmtrx.mtrx[pos], bit);
	return ;
}

void		mtrx_reset(t_source_farm *farm, int i, int j)
{
	register int	pos;
	register int	bit;

	pos = i * farm->bcmtrx.mtrx_len + 1 + j / 64;
	bit = j % 64;
	farm->bcmtrx.mtrx[i * farm->bcmtrx.mtrx_len] -=
		(GETBIT_LLU(farm->bcmtrx.mtrx[pos], bit));
	farm->bcmtrx.mtrx[pos] = RESETBIT_LLU(farm->bcmtrx.mtrx[pos], bit);
	return ;
}

void		mtrx_init(t_source_farm *farm)
{
	register int size;

	size = (int)*farm->rooms.curlen;
	farm->bcmtrx.mtrx_len = (size / 64 + ((size % 64) ? 2 : 1));
	farm->bcmtrx.mtrx = (__uint64_t *)ft_memalloc(8 * size *
												farm->bcmtrx.mtrx_len);
}

void		mtrx_del(t_source_farm *farm)
{
	ft_memdel((void **)&(farm->bcmtrx.mtrx));
	farm->bcmtrx.mtrx_len = 0;
}

void		mtrx_iter_init(t_mtrx_iter *newiter, t_source_farm *farm, int i)
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








t_graph_item	*graph_item(t_work_farm *farm, int i)
{
	return (((t_graph_item **)farm->graph.mem)[i]);
}

t_connect		*list_of_connects(t_work_farm *farm, int i)
{
	return (((t_connect *)&(((t_graph_item **)farm->graph.mem)[i][1])));
}

int				graph(t_work_farm *farm, int i, int j)
{
	return (connect_find(list_of_connects(farm, i),
						graph_item(farm, i)->con_count, j) ? 1 : 0);
}

t_connect		*connect_find(t_connect *connects, int count, int i)
{
	int		bot;
	int		top;
	int		tmp;

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

void		graph_set(t_work_farm *farm, int i, t_connect connect)
{
	int			bot;
	int			top;
	int			tmp; // can be deleted (norminette)
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
		tmp = i - ptr[bot + (top - bot) / 2].dst;
		if (tmp < 0)
			top = bot + (top - bot) / 2;
		else
			bot += (top - bot) / 2 + 1;
	}
	tmp = i - ptr[bot].dst;
	bot += (tmp > 0) ? 1 : 0;
	//FIXME: here ?
	ft_memmove(ptr + bot + 1, ptr + bot,
		sizeof(t_connect) * (GRAPH_ITEM(farm, i)->con_count - bot));
	ptr[bot] = connect;
	return ;
}

void		graph_reset(t_work_farm *farm, int i, int j)
{
	int			bot;
	int			top;
	int			tmp;
	t_connect	*ptr;

	ptr = list_of_connects(farm, i);
	if (!GRAPH_ITEM(farm, i)->con_count)
		return ;
	top = GRAPH_ITEM(farm, i)->con_count - 1;
	bot = 0;
	while (bot != top)
	{
		tmp = i - ptr[bot + (top - bot) / 2].dst;
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
	tmp = i - ptr[bot].dst;
	if (tmp)
		return ;
	ft_memmove(ptr + bot, ptr + bot + 1,
		sizeof(t_connect) * (GRAPH_ITEM(farm, i)->con_count - bot));
	GRAPH_ITEM(farm, i)->con_count--;
	return ;
}

void			graph_iter_init(t_graph_iter *newiter, int i)
{
	newiter->i = 0;
	newiter->row = i;
}

t_connect		*graph_next(t_graph_iter *iter, t_work_farm *farm)
{
	if (iter->i >= GRAPH_ITEM(farm, iter->row)->con_count)
		return (NULL);
	return (&(LIST_OF_CONNECTS(farm, iter->row)[(iter->i)++]));
}
