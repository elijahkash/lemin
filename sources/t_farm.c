/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   t_farm.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtrisha <mtrisha@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/07 11:01:17 by mtrisha           #+#    #+#             */
/*   Updated: 2019/11/08 16:21:26 by mtrisha          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <t_farm.h>

void			farm_init(t_farm *farm)
{
	farm->ants = 0;
	farm->end = -1;
	farm->start = -1;
	farm->way_count = 0;
	farm->mtrx = NULL;
	darr_init(&(farm->rooms), sizeof(char *), 128);
	farm->size = 0;
}

static int		summ(int a, int b)
{
	int res;

	res = 0;
	while (a < b)
		res += a++;
	return (res);
}

static int		mtrx_find(t_farm farm, int i, int j)
{
	int start;
	int bot;
	int top;
	int res;

	start = summ(farm.size - i, farm.size) + i;
	bot = 1;
	top = farm.mtrx[start];
	if (!top)
		return (0);
	while (bot < top)
	{
		res = farm.mtrx[start + bot + (top - bot) / 2] - j;
		if (!res)
			return (bot + (top - bot) / 2);
		if (res < 0)
			bot += (top - bot) / 2 + 1;
		if (res > 0)
			top = bot + (top - bot) / 2 - 1;
	}
	res = farm.mtrx[start + top] - j;
	return (res ? top * -1 : top);
}

char			mtrx(t_farm farm, int i, int j)
{
	int res;

	if (i == j)
		return (farm.mtrx[summ(farm.size - i, farm.size) + i]);
	if (i > j)
		ft_swap(&i, &j, sizeof(int));
	res = mtrx_find(farm, i, j);
	return (res > 0 ? 1 : 0);
}

void			mtrx_set(t_farm farm, int i, int j)
{
	int res;
	int tmp;


	if (i == j)
		return ;
	if (i > j)
		ft_swap(&i, &j, sizeof(int));
	if ((res = mtrx_find(farm, i, j)) > 0)
		return ;
	tmp = summ(farm.size - i, farm.size) + i;
	res *= -1;
	if (!res)
		res = 1;
	else
		res = (farm.mtrx[tmp + res] < j) ? res + 1: res;
	if (farm.mtrx[tmp] && /*res + j < (int)farm.size - 1 && */res < farm.mtrx [tmp] + 1)
		ft_memmove(farm.mtrx + tmp + res + 1, farm.mtrx + tmp + res,
					sizeof(int) * (farm.mtrx[tmp] - (res - 1)));
	farm.mtrx[tmp + res] = j;
	farm.mtrx[tmp]++;
	return ;
}

void			mtrx_init(t_farm *farm)
{
	farm->mtrx = (int *)ft_memalloc(sizeof(int) * (summ(1, farm->size) +
													farm->size));
}

