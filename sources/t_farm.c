/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   t_farm.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtrisha <mtrisha@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/07 11:01:17 by mtrisha           #+#    #+#             */
/*   Updated: 2019/11/11 16:15:30 by mtrisha          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <t_farm.h>

void			farm_init(t_farm *farm)
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
	return (GETBIT(farm.mtrx[i * farm.mtrx_len + 1 + j / sizeof(__uint64_t)],
					sizeof(__uint64_t) - (j % sizeof(__uint64_t))));
}

inline int		mtrx_getcon(t_farm farm, int i)
{
	return ((int)farm.mtrx[i * farm.mtrx_len]);
}

void			mtrx_set(t_farm farm, int i, int j)
{
	//TODO: optimiz calc
	if (i == j)
		return ;
	if (!GETBIT(farm.mtrx[i * farm.mtrx_len + 1 + j / sizeof(__uint64_t)],
					sizeof(__uint64_t) - (j % sizeof(__uint64_t))))
		farm.mtrx[i * farm.mtrx_len]++;
	farm.mtrx[i * farm.mtrx_len + 1 + j / sizeof(__uint64_t)] = SETBIT(
		farm.mtrx[i * farm.mtrx_len + 1 + j / sizeof(__uint64_t)],
		sizeof(__uint64_t) - (j % sizeof(__uint64_t)));
	if (!GETBIT(farm.mtrx[j * farm.mtrx_len + 1 + i / sizeof(__uint64_t)],
					sizeof(__uint64_t) - (i % sizeof(__uint64_t))))
		farm.mtrx[j * farm.mtrx_len]++;
	farm.mtrx[j * farm.mtrx_len + 1 + i / sizeof(__uint64_t)] = SETBIT(
		farm.mtrx[j * farm.mtrx_len + 1 + i / sizeof(__uint64_t)],
		sizeof(__uint64_t) - (i % sizeof(__uint64_t)));
	return ;
}

void			mtrx_init(t_farm *farm)
{
	farm->mtrx_len = farm->size / (8 * sizeof(__uint64_t));
	farm->mtrx_len += (farm->size % (8 * sizeof(__uint64_t))) ? 2 : 1;
	farm->mtrx = (__uint64_t *)ft_memalloc(sizeof(__uint64_t) *
												farm->size * farm->mtrx_len);
}

void			iter_init(t_iter *newiter, t_farm farm, int i)
{
	//newiter = (t_iter *)ft_malloc(sizeof(t_iter));
	newiter->row = i;
	newiter->next = 0;
	newiter->i = 1;
	newiter->curitem = farm.mtrx[i * farm.mtrx_len + 1];
	newiter->least = farm.mtrx[i * farm.mtrx_len];
}

int				next(t_iter *iter, t_farm farm)
{
	__uint64_t res;

	if (!iter->least)
		return (-1);
	while (!iter->curitem)
		iter->curitem = farm.mtrx[iter->row * farm.mtrx_len + ++(iter->i)];
	iter->least--;
	res = iter->curitem;
	asm("bsrq\t%1, %0" : "=r" (res) , "+rm" (res));
	iter->curitem = RESETBIT(iter->curitem, res + 1);
	return (iter->next = sizeof(__uint64_t) * 8 * (iter->i - 1) +
							8 * (res / 8 + 1) - (int)res - 1);
}
