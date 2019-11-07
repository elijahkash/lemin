/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   t_farm.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtrisha <mtrisha@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/07 11:01:17 by mtrisha           #+#    #+#             */
/*   Updated: 2019/11/07 12:46:29 by mtrisha          ###   ########.fr       */
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
}

static int		summ(int a, int b)
{
	int res;

	res = 0;
	while (a < b)
		res += a++;
	return (res);
}

char			mtrx(t_farm farm, int i, int j)
{
	if (i == j)
		return (0);
	if (i > j)
		ft_swap(&i, &j, sizeof(int));
	return (farm.mtrx[summ(darr_l(farm.rooms) - i, darr_l(farm.rooms)) +
						j - i - 1]);
}

void			mtrx_init(t_farm *farm)
{
	farm->mtrx = (char *)ft_malloc(sizeof(char) *
									summ(1, darr_l(farm->rooms)));
}

