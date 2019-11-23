/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   solve.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtrisha <mtrisha@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/07 12:56:50 by mtrisha           #+#    #+#             */
/*   Updated: 2019/11/23 19:46:19 by mtrisha          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <solve.h>






#include <stdio.h>






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
	if (!(GRAPH_ITEM(farm, connect.dst)->state & SEPARATE))
	{
		if (connect.dst != farm->start)
			GRAPH_ITEM(farm, connect.dst)->state |= (SEPARATE | MARKED_OUT);
		graph_connect(farm, connect.dst, connect.src)->state |= WAY_FORBIDDEN;
		graph_connect(farm, connect.src, connect.dst)->state |= WAY_NEGATIVE;
	}
	else
	{
		GRAPH_ITEM(farm, connect.dst)->state &= ~(SEPARATE | MARKED_IN);
		graph_connect(farm, connect.dst, connect.src)->state |= WAY_BASE_STATE;
		graph_connect(farm, connect.src, connect.dst)->state |= WAY_BASE_STATE;
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
		while ((j = graph_next(&iter, farm))) // _need parse forbidden?
			if (GRAPH_ITEM(farm, j->dst)->weight == k &&
				(h = graph_state(farm, j->dst, i) && !(h & WAY_FORBIDDEN)))
				break ;
		connect.dst = j->dst;
		connect.src = i;
		i = connect.dst;
		darr_add(list_results, &connect);
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



	printf("\n!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");


	t_graph_iter iter2;
	t_connect *ptr;
	i = 0;
	while (i < farm->graph.size)
	{
		printf("%d\t%s\t%d\t", i, (char *)darr(farm->rooms,
			GRAPH_ITEM(farm, i)->id), GRAPH_ITEM(farm, i)->con_count);
		graph_iter_init(&iter2, i, 0);
		while ((ptr = graph_next(&iter2, farm)))
		{
			printf("%2d", ptr->dst * ((ptr->state & WAY_NEGATIVE) ? -1 : 1));
			printf("%1s", ((ptr->state & WAY_FORBIDDEN) ? "*" : ""));
		}
		printf("\n");
		i++;
	}
	printf("\n\n%d\n\n", i);



	darr_del(&marked);
	return (1);
}

int		solve(t_work_farm *farm)
{
	t_darr	list_results;

	darr_init(&list_results, sizeof(t_full_connect), 64);
	while (find_new_way(list_results, farm))
		continue ;

	size_t i;

	i = 0;
	while (i < darr_l(list_results))
	{
		t_full_connect *tmp;

		tmp = darr(list_results, i);
		ft_printf("%d-%d\n", tmp->src, tmp->dst);
		i++;
	}


	darr_del(&list_results);
	return (0);
}
