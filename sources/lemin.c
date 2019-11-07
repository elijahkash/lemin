/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lemin.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtrisha <mtrisha@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/06 16:20:37 by mtrisha           #+#    #+#             */
/*   Updated: 2019/11/07 13:23:22 by mtrisha          ###   ########.fr       */
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
	else if (farm.ants == 0)
		ft_printf("There no ants!\n");
	else if (darr_l(farm.rooms) == 0)
		ft_printf("There no rooms!\n");
	else if (farm.start == 0 || farm.end == 0)
		ft_printf("There no start/end room!\n");
	if (ret || !farm.ants || !darr_l(farm.rooms) || !farm.start || !farm.end)
		return ;
	ret = solve(farm);
	if (ret)
		ft_printf("There no way between start and end!\n");
	return ;
}
