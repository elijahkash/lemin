/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lemin.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtrisha <mtrisha@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/09 15:10:20 by mtrisha           #+#    #+#             */
/*   Updated: 2019/12/10 11:56:50 by mtrisha          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <lemin.h>

#include <farm.h>
#include <read_input.h>
#include <input_errors.h>
#include <solve.h>

static const t_err	g_errlist[] =
{
	{ERRSTATE, "unknown error"},
	{NO_POSSIBLE_WAY, "There's no way between start and end!"},
	{NO_START_END, "There's no start/end room!"},
	{NO_TUBES, "There's no tubes!"},
	{NO_ROOMS, "There's no rooms!"},
	{NO_ANTS, "There's no ants!"},
	{SAME_WAYS, "Found several identical tubes."},
	{TUBE_ERROR, "Wrong tube definition."},
	{NO_UNIQ, "Found several rooms with identical names."},
	{TOO_MUCH, "Too much input rooms."},
	{ROOM_ERROR, "Wrong room definition."},
	{ANTS_ERROR, "Wrong ants definition."},
	{WRONG_CMD, "Wrong command lines."},
	{GNL_ERROR, "Reading input stream return error."}
};

void				print_input_error(t_uint ret)
{
	const char	*err_str;
	t_uint 		i;

	i = (sizeof(g_errlist) / sizeof(t_err));
	while (i-- > 0 )
		if (g_errlist[i].error_code & ret)
		{
			err_str = g_errlist[i].error_string;
			break ;
		}
	ft_printf("ERROR: \"%s\"\n", err_str);
}

int					detect_errors(t_uint ret, t_farm *farm)
{
	if (farm->ants == 0)
		ret |= NO_ANTS;
	else if (farm->names.curlen == 0)
		ret |= NO_ROOMS;
	else if (farm->connects.curlen == 0)
		ret |= NO_TUBES;
	else if (farm->start == FARM_INIT_SE_VALUES ||
				farm->end == FARM_INIT_SE_VALUES)
		ret |= NO_START_END;
	return (ret);
}

void				lemin(void)
{
	t_farm		farm[1];
	int			ret;
	t_enum_ways	result;

	farm_init(farm);
	ret = read_input(farm);
	ft_force_buff();
	if (farm->connects.mem)
		vect_shrink(&(farm->connects), 0);
	if ((ret |= detect_errors(ret, farm)) ||
		(ret |= graph_init(&(farm->graph), farm)))
	{
		print_input_error(ret);
		farm_del(farm);
		return ;
	}


	// t_iter iter;
	// t_connect *tmp;
	// for (t_uint j = 0; j < farm->graph.size; j++)
	// {
	// 	ft_printf("%s\t%lu\t", *(char **)vect(&(farm->names), j), farm->graph.nodes[j]->count_connects);
	// 	iter_init(&iter, farm->graph.nodes[j], ITER_ALL);
	// 	while ((tmp = iter_next(&iter)))
	// 	{
	// 		ft_printf("%s ", *(char **)vect(&(farm->names), tmp->dst));
	// 	}
	// 	ft_printf("\n");
	// }
	// ft_printf("\n");

	// ft_force_buff();

	farm_del_connects(farm);
	ft_bzero(&result, sizeof(t_enum_ways));
	solve(&result, &(farm->graph), farm->ants);
	graph_del(&(farm->graph));
	if (result.count)
		print_result(&result, farm->ants);
	else
		print_input_error(NO_POSSIBLE_WAY);


	// ft_printf("ways = %d\n", result.count);
	// t_darr	test;

	// darr_init(&test, 4, 256);
	// for (t_uint j = 0; j < result.count; j++)
	// {
	// 	ft_printf("len = %d\t", result.ways[j].len);
	// 	for(t_uint i = 0; i < result.ways[j].len; i++)
	// 	{
	// 		ft_printf(" %s", vect(&(farm->names), result.ways[j].nodes[i]));
	// 		if (result.ways[j].nodes[i] != farm->graph.end)
	// 		{
	// 			for(int k = 0; k < (int)darr_l(test); k++)
	// 				if (result.ways[j].nodes[i] == *(t_uint *)darr(test, k))
	// 					ft_printf("*");
	// 		}
	// 		darr_add(test, &(result.ways[j].nodes[i]));
	// 	}
	// 	ft_printf("\n");
	// }
	// ft_printf("\n");

	enum_ways_del(&result);
	farm_del(farm);
	return ;
}
