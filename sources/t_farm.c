/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   t_farm.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtrisha <mtrisha@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/07 11:01:17 by mtrisha           #+#    #+#             */
/*   Updated: 2019/11/14 18:22:35 by mtrisha          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <t_farm.h>

void		farm_init(t_farm *farm)
{
	farm->ants = 0;
	farm->end = -1;
	farm->start = -1;
	farm->bcmtrx.mtrx_len = 0;
	farm->bcmtrx.mtrx = NULL;
	farm->work_graph.mem = NULL;
	farm->work_graph.size = 0;
	darr_init(&(farm->rooms), PTR_SIZE, 128);
}

int			mtrx(t_farm *farm, int i, int j)
{
	return (GETBIT_LLU(
		farm->bcmtrx.mtrx[i * farm->bcmtrx.mtrx_len + 1 + j / 64], j % 64));
}

int		mtrx_getcon(t_farm *farm, int i)
{
	return ((int)farm->bcmtrx.mtrx[i * farm->bcmtrx.mtrx_len]);
}

void		mtrx_set(t_farm *farm, int i, int j)
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

void		mtrx_reset(t_farm *farm, int i, int j)
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

void		mtrx_init(t_farm *farm)
{
	register int size;

	size = (int)*farm->rooms.curlen;
	farm->bcmtrx.mtrx_len = (size / 64 + ((size % 64) ? 2 : 1));
	farm->bcmtrx.mtrx = (__uint64_t *)ft_memalloc(8 * size *
												farm->bcmtrx.mtrx_len);
}

void		mtrx_iter_init(t_mtrx_iter *newiter, t_farm *farm, int i)
{
	newiter->row = i;
	newiter->i = 1;
	newiter->curitem = farm->bcmtrx.mtrx[i * farm->bcmtrx.mtrx_len + 1];
	newiter->least = farm->bcmtrx.mtrx[i * farm->bcmtrx.mtrx_len];
}

int			mtrx_next(t_mtrx_iter *iter, t_farm *farm)
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

void		iter_init(t_iter *newiter, int i)
{
	newiter->i = 0;
	newiter->row = i;
}

t_connect	*next(t_iter *iter, t_farm *farm)
{
	if (iter->i >= GRAPH_ITEM(iter->row).con_count)
		return (NULL);
	return (&(((t_connect *)&(((t_graph_item **)farm->work_graph.mem)
						[iter->row][1]))[(iter->i)++]));
}
