/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lemin.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmathew <hmathew@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/09 15:10:20 by mtrisha           #+#    #+#             */
/*   Updated: 2019/12/09 20:29:40 by hmathew          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <lemin.h>

#include <farm.h>
#include <read_input.h>
#include <input_errors.h>
#include <solve.h>
#include <string.h>

void	print_input_error(int ret, t_farm *farm)
{
	const char	*err_str = "unknown error";
	int 		i;

	if (!ret)
	{
		if (farm->ants == 0)
			ret = NO_ANTS;
		else if (farm->names.curlen == 0)
			ret = NO_ROOMS;
		else if (farm->connects.curlen == 0)
			ret = NO_TUBES;
		else if (farm->start == (size_t)(0 - 1) || farm->end == (size_t)(0 - 1))
			ret = NO_START_END;
	}
	i = 0;
	while (i < (sizeof(g_errlist) / sizeof(t_err)))
	{
		if (g_errlist[i].error_code == ret)
		{
			err_str = g_errlist[i].error_string;
			break ;
		}
		++i;
	}
	printf("ERROR: %s\n", err_str);
}

int		detect_errors(int ret, t_farm *farm)
{
	if (ret || farm->ants == 0 || farm->names.curlen == 0 ||
		farm->start == (size_t)(0 - 1) || farm->end == (size_t)(0 - 1) ||
		farm->connects.curlen == 0)
		return (1);
	else
		return (0);
}

void	lemin(void)
{
	t_farm		farm[1];
	int			ret;
	t_enum_ways	result;

	farm_init(farm);
	ret = read_input(farm);
	ft_force_buff();
	if (farm->connects.mem)
		vect_shrink(&(farm->connects), 0);
	if (detect_errors(ret, farm) || (ret |= graph_init(&(farm->graph), farm)))
	{
		print_input_error(ret, farm);
		farm_del(farm);
		return ;
	}
	farm_del_connects(farm);
	ft_bzero(&result, sizeof(t_enum_ways));
	solve(&result, &(farm->graph), farm->ants);
	graph_del(&(farm->graph));
	if (result.count)
		print_result(&result, farm->ants);
	else
		ft_printf("There's no way between start and end!\n");
	enum_ways_del(&result);
	farm_del(farm);
	return ;
}
