/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtrisha <mtrisha@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/29 17:09:56 by mtrisha           #+#    #+#             */
/*   Updated: 2019/12/06 11:22:45 by mtrisha          ###   ########.fr       */
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
