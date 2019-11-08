/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lemin.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtrisha <mtrisha@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/06 16:20:37 by mtrisha           #+#    #+#             */
/*   Updated: 2019/11/08 13:20:30 by mtrisha          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <lemin.h>

#include <t_farm.h>
#include <handle_input.h>
#include <solve.h>

void	lemin(void)
{
	t_farm	farm;
	int		ret;

	farm_init(&farm);
	ret = handle_input(&farm);
	if (ret)
		ft_printf("ERROR\n");
	else if (!farm.ants)
		ft_printf("There's no ants!\n");
	else if (!farm.size)
		ft_printf("There's no rooms!\n");
	else if (farm.start == -1 || farm.end == -1)
		ft_printf("There's no start/end room!\n");
	else if (farm.mtrx == NULL)
		ft_printf("There's no tubes!\n");
	if (ret || !(farm.ants * farm.size) || farm.start == -1 || farm.end == -1
			|| !farm.mtrx)
		return ;
	ret = solve(farm);
	if (ret)
		ft_printf("There's no way between start and end!\n");



	int i;
	int j;
	i = 0;
	while (i < (int)farm.size)
	{
		j = 0;
		ft_printf("%s\t", *(char **)darr(farm.rooms, i));
		while (j < (int)farm.size)
			ft_printf("%d ", mtrx(farm, i, j++));
		ft_printf("\n");
		i++;
	}



	return ;
}
