/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Kashnitskiy <elijahkash.code@gmail.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/29 17:09:56 by mtrisha           #+#    #+#             */
/*   Updated: 2020/01/27 17:06:53 by Kashnitskiy      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>

#include <lemin.h>

int		main(void)
{
	ft_memman_init();
	lemin();
	ft_force_buff();
	ft_memman_clean();
	return (0);
}
