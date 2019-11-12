/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   t_farm.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtrisha <mtrisha@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/07 11:01:17 by mtrisha           #+#    #+#             */
/*   Updated: 2019/11/12 11:54:50 by mtrisha          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <t_farm.h>

void			farm_init(t_farm *restrict farm)
{
	farm->ants = 0;
	farm->end = -1;
	farm->start = -1;
	farm->mtrx_len = 0;
	farm->mtrx = NULL;
	darr_init(&(farm->rooms), sizeof(char *), 128);
	farm->size = 0;
}

inline int		mtrx(t_farm farm, int i, int j)
{
	return (GETBIT_LLU(farm.mtrx[i * farm.mtrx_len + 1 + j / 64], j % 64));
}

inline int		mtrx_getcon(t_farm farm, int i)
{
	return ((int)farm.mtrx[i * farm.mtrx_len]);
}

void			mtrx_set(t_farm farm, int i, int j)
{
	register int	pos;
	register int	bit;

	pos = i * farm.mtrx_len + 1 + j / 64;
	bit = j % 64;
	farm.mtrx[i * farm.mtrx_len] += (!GETBIT_LLU(farm.mtrx[pos], bit));
	farm.mtrx[pos] = SETBIT_LLU(farm.mtrx[pos], bit);
	return ;
}

void			mtrx_init(t_farm *restrict farm)
{
	farm->mtrx_len = farm->size / 64 + (farm->size % 64) ? 2 : 1;
	farm->mtrx = (__uint64_t *)ft_memalloc(8 * farm->size * farm->mtrx_len);
}

void			iter_init(t_iter *restrict newiter, t_farm farm, register int i)
{
	newiter->row = i;
	newiter->i = 1;
	newiter->curitem = farm.mtrx[i * farm.mtrx_len + 1];
	newiter->least = farm.mtrx[i * farm.mtrx_len];
}

int				next(t_iter *restrict iter, t_farm farm)
{
	register __uint64_t tmp;

	if (!iter->least)
		return (-1);
	while (!iter->curitem)
		iter->curitem = farm.mtrx[iter->row * farm.mtrx_len + ++(iter->i)];
	iter->least--;
	tmp = iter->curitem;
	BSR_ASM(tmp);
	iter->curitem = RESETBIT_LLU(iter->curitem, tmp);
	return (64 * (iter->i - 1) + (int)tmp);
}
