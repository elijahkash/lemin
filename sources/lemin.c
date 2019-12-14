/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lemin.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtrisha <mtrisha@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/09 15:10:20 by mtrisha           #+#    #+#             */
/*   Updated: 2019/12/14 20:07:40 by mtrisha          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <lemin.h>

#include <farm.h>
#include <read_input.h>
#include <input_errors.h>
#include <solve.h>

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
	t_uint		ret;
	t_enum_ways	result;

	farm_init(farm);
	ret = read_input(farm);
	ft_force_buff();
	if ((ret |= detect_errors(ret, farm)) ||
		(ret |= graph_init(&(farm->graph), farm)))
	{
		print_input_error(ret);
		farm_del(farm);
		return ;
	}
	farm_del_connects(farm);
	ft_bzero(&result, sizeof(t_enum_ways));
	solve(&result, &(farm->graph), farm->ants);
	graph_del(&(farm->graph));
	result.count ? print_result(&result, farm) :
					print_input_error(NO_POSSIBLE_WAY);
	enum_ways_del(&result);
	farm_del(farm);
	return ;
}

//TODO: init values (+ deq etc)
//TODO: OWERFLOW ANTS?
//TODO: breakpoint in solve ?

/*
	t_uint	connect_dst;
	t_uint	connect_state;
	t_uint	node_con;
	t_uint	node_marked;
	t_uint	node_sep;
	t_uint	node_in;
	t_uint	node_out;
	t_uint	node_bfs;

	node_con = node->count_connects;
	node_marked = node->marked;
	node_sep = node->separate;
	node_in = node->marked_in;
	node_out = node->marked_out;
	node_bfs = node->bfs_level;

	connect_dst = connect->dst;
	connect_state = connect->state;
*/
