/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   solve.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtrisha <mtrisha@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/07 12:56:50 by mtrisha           #+#    #+#             */
/*   Updated: 2019/12/04 18:45:50 by mtrisha          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <solve.h>

void	mark_item(t_connect *connect, t_work_farm *farm, __int32_t weight)
{
	if (!(GRAPH_ITEM(farm, connect->dst)->state & SEPARATE))
	{
		GRAPH_ITEM(farm, connect->dst)->state |= MARKED;
		GRAPH_ITEM(farm, connect->dst)->weight = weight + 1;
	}
	else
	{
		GRAPH_ITEM(farm, connect->dst)->state |= MARKED;
		// GRAPH_ITEM(farm, connect->dst)->weight = weight +
		// 	((!(connect->state & WAY_NEGATIVE)) ? 1 : -1);
		GRAPH_ITEM(farm, connect->dst)->weight = weight + 1;
		GRAPH_ITEM(farm, connect->dst)->state |=
			((!(connect->state & WAY_NEGATIVE)) ?
					MARKED_IN : MARKED_OUT);
	}
}

void	add_nodes(__int32_t item, t_work_farm *farm, t_darr marked)
{
	t_graph_iter iter;
	register char tmp; //!!
	t_connect	*k;

	graph_iter_init(&iter, item, farm);
	tmp = GRAPH_ITEM(farm, item)->state; //!
	while ((k = graph_next(&iter, farm)))
	{
		if (!(GRAPH_ITEM(farm, k->dst)->state & MARKED))
		{
			darr_add(marked, &k->dst);
			mark_item(k, farm, GRAPH_ITEM(farm, item)->weight);
		}
	}
}

void	reverse(t_full_connect connect, t_work_farm *farm)
{
	if (graph_connect(farm, connect.src, connect.dst)->state & WAY_FORBIDDEN)
	{
		graph_connect(farm, connect.dst, connect.src)->state = WAY_BASE_STATE;
		graph_connect(farm, connect.src, connect.dst)->state = WAY_BASE_STATE;
	}
	else
	{
		graph_connect(farm, connect.dst, connect.src)->state = WAY_FORBIDDEN;
		graph_connect(farm, connect.src, connect.dst)->state = WAY_NEGATIVE;
	}
	if (!(GRAPH_ITEM(farm, connect.dst)->state & SEPARATE))
	{
		if (connect.dst != farm->start)
			GRAPH_ITEM(farm, connect.dst)->state |= (SEPARATE | MARKED_OUT);
	}
	else
	{
		t_graph_iter iter;

		graph_iter_init(&iter, connect.dst, farm);
		iter.state = NEG_WAYS;
		if (graph_next(&iter, farm) == NULL)
			// GRAPH_ITEM(farm, connect.dst)->state &= ~(SEPARATE | MARKED_IN);
		GRAPH_ITEM(farm, connect.dst)->state = MARKED;
	}


}

int		find_new_way(t_darr list_results, t_work_farm *farm)
{
	t_darr	marked; //TODO: here must be dec???

	darr_init(&marked, 4, 64);
	clean_graph_state(farm);


	darr_add(marked, &farm->start);
	GRAPH_ITEM(farm, farm->start)->state = MARKED;
	GRAPH_ITEM(farm, farm->start)->weight = 0;

	__int32_t	i;
	i = 0;
	while (i < (int)darr_l(marked)) //__int32 ????
	{
		add_nodes(*(__int32_t *)darr(marked, i), farm, marked);
		i++;
		if (GRAPH_ITEM(farm, farm->end)->state & MARKED) // wrong stop if? -1 ?
			break ;
	}


	if (!(GRAPH_ITEM(farm, farm->end)->state & MARKED))
		return (0); // darr_del etc




	__int32_t	k;
	t_connect	*j;
	int			h;
	t_graph_iter iter;
	t_full_connect connect;
	i = farm->end;
	k = GRAPH_ITEM(farm, farm->end)->weight;
	while (k-- > 0)
	{
		graph_iter_init(&iter, i, 0);
		while ((j = graph_next(&iter, farm)))
			if (GRAPH_ITEM(farm, j->dst)->weight == k &&
	((h = graph_state(farm, j->dst, i)) && !(h & WAY_FORBIDDEN) &&
	(!(graph_item(farm, j->dst)->state & SEPARATE) ||
	(graph_item(farm, j->dst)->state & MARKED_IN && h & WAY_NEGATIVE) ||
	graph_item(farm, j->dst)->state & MARKED_OUT)))
				break ;
		// if (i == farm->end)
		// 	ft_printf(" %s ", (char *)darr(farm->rooms, graph_item(farm, j->dst)->id));
		connect.dst = j->dst;
		connect.src = i;
		i = connect.dst;
		// darr_add(list_results, &connect);
		// if (k > 0)
			reverse(connect, farm);
	}





	// 	i = 0;
	// while (i < (__int32_t)darr_l(list_results))
	// {
	// 	t_full_connect *tmp;

	// 	tmp = darr(list_results, i);
	// 	ft_printf("%d-%d\n", tmp->src, tmp->dst);
	// 	i++;
	// }



	// printf("\n!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");


	// t_graph_iter iter2;
	// t_connect *ptr;
	// i = 0;
	// while (i < farm->graph.size)
	// {
	// 	printf("%d\t%s\t%d\t", i, (char *)darr(farm->rooms,
	// 		GRAPH_ITEM(farm, i)->id), GRAPH_ITEM(farm, i)->con_count);
	// 	graph_iter_init(&iter2, i, 0);
	// 	while ((ptr = graph_next(&iter2, farm)))
	// 	{
	// 		printf("%2d", ptr->dst * ((ptr->state & WAY_NEGATIVE) ? -1 : 1));
	// 		printf("%1s", ((ptr->state & WAY_FORBIDDEN) ? "*" : ""));
	// 	}
	// 	printf("\n");
	// 	i++;
	// }
	// printf("\n\n%d\n\n", i);



	darr_del(&marked);
	return (1);
}

typedef struct	s_way
{
	__int32_t	*connects;
	__int32_t	len;
	__int32_t		ants; //TODO: size_t!!!!!!!!!!!???????????????
	__int32_t	border;
}				t_way;

typedef struct	s_list_ways
{
	t_way		*ways;
	__int32_t	count;
}				t_list_ways;

int		comp_way_by_len(const void *a, const void *b)
{
	return (((t_way *)a)->len - ((t_way *)b)->len);
}

void	find_way(t_darr way, __int32_t first, t_work_farm *farm)
{
	t_graph_iter	iter;
	t_connect		*tmp;

	darr_add(way, &first);
	graph_iter_init(&iter, first, farm);
	iter.state = FORB_WAYS;
	while (*(__int32_t *)darr_top(way) != farm->end)
	{
		tmp = graph_next(&iter, farm);
		darr_add(way, &(tmp->dst));
		graph_iter_init(&iter, tmp->dst, farm);
		iter.state = FORB_WAYS;
	}
}

void	find_ways(t_list_ways *res, t_work_farm *farm)
{
	__int32_t	i;
	t_graph_iter	iter;
	t_connect		*tmp;
	t_darr			way; //del


	darr_init(&way, sizeof(__int32_t), 32);
	i = 0;
	res->ways = ft_malloc(sizeof(t_way) * res->count);
	graph_iter_init(&iter, farm->start, 0);
	iter.state = FORB_WAYS; //!!!!!!!!!!!!!
	while ((tmp = graph_next(&iter, farm)))
	{
		darr_clean(way);
		//darr_add(way, &farm->start);
		find_way(way, tmp->dst, farm);
		res->ways[i].len = darr_l(way);
		res->ways[i].connects = ft_malloc(4 * res->ways[i].len);
		for (int j = 0; j < (int)darr_l(way); j++)
			res->ways[i].connects[j] = *(__int32_t *)darr(way, j);
		i++;
	}
	ft_qsort(res->ways, res->count, sizeof(t_way), comp_way_by_len);
}

__int32_t	calc_moves(t_list_ways res, __int32_t ants)
{
	int k = 0;
	int tmp;
	int way = res.count;

	if (res.count == 1) //!
		return (ants + res.ways[0].len - 1);
	for(int i = 0; i < res.count ; i++)
	{
		res.ways[i].border = 0;
		// if (i == 0)
		// 	res.ways[0].border = res.ways[0].len;
		for (int j = 0; j < i; j++)
			res.ways[i].border += (res.ways[i].len - res.ways[j].len);
	}
	while (way > 0)
	{
		if (ants > res.ways[way - 1].border)
		{
			k++;
			ants -= way;
		}
		else
		{
			way--;
			res.ways[way].ants = k;
		}

		// tmp = (way != 1) ? (ants / (res.ways[way - 1].border)) : ants;
		// if (tmp)
		// {
		// 	k += tmp;
		// 	ants -= (tmp * way);
		// }
		// way--;
		// res.ways[way].ants = k;
	}
	return (res.ways[0].ants + res.ways[0].len - 1);
}

int		solve(t_work_farm *farm)
{
	t_darr	list_results;
	__int32_t	k;
	__int32_t	min_moves; //size_t?
	t_list_ways	res;
	t_list_ways	tmp;

	darr_init(&list_results, sizeof(t_full_connect), 64);
	k = 0;
	min_moves = 0;
	while (find_new_way(list_results, farm))
	{
		k++;
		tmp.count = k;
		find_ways(&tmp, farm);
		if (k == 1 || (calc_moves(tmp, farm->ants) < min_moves))
		{
			min_moves = calc_moves(tmp, farm->ants);
			res = tmp;
		}


	// ft_printf("\nnumber of ways = %d\n", res.count);
	// ft_printf("number of moves = %d\n", min_moves);
	// ft_printf("number of tmp!!!ways = %d\n", tmp.count);
	// ft_printf("number of tmp!!!moves = %d\n", calc_moves(tmp, farm->ants));

		//del tmp/res

	ft_printf("ways = %d\n", res.count);
	t_darr	test;

	darr_init(&test, 4, 256);
	for (int j = 0; j < res.count; j++)
	{
		ft_printf("len = %d\t", res.ways[j].len);
		for(int i = 0; i < res.ways[j].len; i++)
		{
			ft_printf(" %s", darr(farm->rooms, GRAPH_ITEM(farm, res.ways[j].connects[i])->id));
			//ft_printf(" %d", res.ways[j].connects[i]);
			// if (i < res.ways[j].len - 1 && !(graph_connect(farm, res.ways[j].connects[i], res.ways[j].connects[i + 1])->state & WAY_FORBIDDEN))
			// 	ft_printf("=");
			// else
			// 	ft_printf("");
			if (res.ways[j].connects[i] != farm->end)
			{
				for(int k = 0; k < (int)darr_l(test); k++)
					if (res.ways[j].connects[i] == *(int *)darr(test, k))
						ft_printf("*");
			}
			darr_add(test, &(res.ways[j].connects[i]));
		}
		ft_printf("\n");
	}
	ft_printf("\n\n\n");
	}

	// ft_printf("\nlen0=%d\n", res.ways[0].len);
	// for(int i = 0; i < res.ways[0].len; i++)
	// 	ft_printf(" %d", res.ways[0].connects[i]);
	// ft_printf("\nlen1=%d\n", res.ways[1].len);
	// for(int i = 0; i < res.ways[1].len; i++)
	// 	ft_printf(" %d", res.ways[1].connects[i]);



	ft_printf("\nnumber of ways = %d\n", res.count);
	ft_printf("\nnumber of moves = %d\n", min_moves);
	ft_printf("\nnumber of tmp!!!ways = %d\n", tmp.count);
	ft_printf("\nnumber of tmp!!!moves = %d\n", calc_moves(tmp, farm->ants));



	// size_t i;

	// i = 0;
	// while (i < darr_l(list_results))
	// {
	// 	t_full_connect *tmp;

	// 	tmp = darr(list_results, i);
	// 	ft_printf("%d-%d\n", tmp->src, tmp->dst);
	// 	i++;
	// }


	darr_del(&list_results);
	return (0);
}
