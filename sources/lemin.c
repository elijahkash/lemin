/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lemin.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtrisha <mtrisha@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/09 15:10:20 by mtrisha           #+#    #+#             */
/*   Updated: 2019/12/09 15:47:49 by mtrisha          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <lemin.h>

#include <farm.h>
#include <read_input.h>
#include <input_errors.h>
#include <solve.h>

void	print_input_error(int ret, t_farm *farm)
{
	if (ret)
	{
		if (ret & WRONG_CMD)
			ft_printf("ERROR: wrong command lines.\n");
		else if (ret & ANTS_ERROR)
			ft_printf("ERROR: wrong ants definition.\n");
		else if (ret & ROOM_ERROR)
			ft_printf("ERROR: wrong room definition.\n");
		else if (ret & TOO_MUCH)
			ft_printf("ERROR: too much input rooms.\n");
		else if (ret & GNL_ERROR)
			ft_printf("ERROR: read input stream return error.\n");
		else if (ret & NO_UNIQ)
			ft_printf("ERROR: found several rooms with identical names.\n");
		else if (ret & TUBE_ERROR)
			ft_printf("ERROR: wrong tube definition.\n");
		else if (ret & SAME_WAYS)
			ft_printf("ERROR: found several identical tubes.\n");
		else
			ft_printf("ERROR\n");
	}
	else if (farm->ants == 0)
		ft_printf("There's no ants!\n");
	else if (farm->names.curlen == 0)
		ft_printf("There's no rooms!\n");
	else if (farm->connects.curlen == 0)
		ft_printf("There's no tubes!\n");
	else if (farm->start == 0 - 1 || farm->end == 0 - 1)
		ft_printf("There's no start/end room!\n");
}

int		detect_errors(int ret, t_farm *farm)
{
	if (ret || farm->ants == 0 || farm->names.curlen == 0 ||
	farm->start == 0 - 1 || farm->end == 0 - 1 || farm->connects.curlen == 0)
		return (1);
	else
		return (0);
}

void	lemin(void)
{
	t_farm		farm[1];
	int			ret;
	t_enum_ways	*result;

	farm_init(farm);
	ret = read_input(farm);
	ft_force_buff();
	if (detect_errors(ret, farm) || (ret |= graph_init(&(farm->graph), farm)))
	{
		print_input_error(ret, farm);
		farm_del(farm);
		return ;
	}
	farm_del_connects(farm);
	result = solve(farm);
	graph_del(&(farm->graph));
	print_result(result);
	farm_del(&farm);
	return ;
}
