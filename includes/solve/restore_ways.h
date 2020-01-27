/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   restore_ways.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Kashnitskiy <elijahkash.code@gmail.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/09 14:01:22 by mtrisha           #+#    #+#             */
/*   Updated: 2020/01/27 17:06:22 by Kashnitskiy      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RESTORE_WAYS_H
# define RESTORE_WAYS_H

# include <graph.h>
# include <ways.h>

void	restore_ways(t_enum_ways *res, t_graph *graph, t_uint ways_count);

#endif
