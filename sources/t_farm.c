/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   t_farm.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtrisha <mtrisha@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/07 11:01:17 by mtrisha           #+#    #+#             */
/*   Updated: 2019/11/13 21:18:57 by mtrisha          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <t_farm.h>

void			farm_init(t_farm *restrict farm)
{
	farm->ants = 0;
	farm->end = -1;
	farm->start = -1;
	farm->bcmtrx.mtrx_len = 0;
	farm->bcmtrx.mtrx = NULL;
	farm->work_graph.mem = NULL;
	farm->work_graph.size = 0;
	darr_init(&(farm->rooms), sizeof(char *), 128);
}

inline int		mtrx(t_farm *restrict farm, int i, int j)
{
	return (GETBIT_LLU(
		farm->bcmtrx.mtrx[i * farm->bcmtrx.mtrx_len + 1 + j / 64], j % 64));
}

inline int		mtrx_getcon(t_farm *restrict farm, int i)
{
	return ((int)farm->bcmtrx.mtrx[i * farm->bcmtrx.mtrx_len]);
}

inline void		mtrx_set(t_farm *restrict farm, int i, int j)
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

inline void		mtrx_reset(t_farm *restrict farm, int i, int j)
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

inline void		mtrx_init(t_farm *restrict farm)
{
	register int size;

	size = (int)*farm->rooms.curlen;
	farm->bcmtrx.mtrx_len = (size / 64 + ((size % 64) ? 2 : 1));
	farm->bcmtrx.mtrx = (__uint64_t *)ft_memalloc(8 * size *
												farm->bcmtrx.mtrx_len);
}

inline void		iter_init(t_iter *restrict newiter, t_farm *restrict farm,
							register int i)
{
	newiter->row = i;
	newiter->i = 1;
	newiter->curitem = farm->bcmtrx.mtrx[i * farm->bcmtrx.mtrx_len + 1];
	newiter->least = farm->bcmtrx.mtrx[i * farm->bcmtrx.mtrx_len];
}

int				next(t_iter *restrict iter, t_farm *restrict farm)
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
