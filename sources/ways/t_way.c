/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   t_way.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Kashnitskiy <elijahkash.code@gmail.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/09 15:02:14 by mtrisha           #+#    #+#             */
/*   Updated: 2020/01/27 17:06:57 by Kashnitskiy      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ways.h>

inline void				way_init(t_way *restrict way, t_uint *restrict arr,
									t_uint len)
{
	way->ants = 0;
	way->len = len;
	way->ants_names = NULL;
	way->nodes = arr;
}

inline int				comp_way_by_len(const void *restrict way1,
										const void *restrict way2)
{
	return (((t_way *)way1)->len - ((t_way *)way2)->len);
}
