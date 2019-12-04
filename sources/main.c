/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtrisha <mtrisha@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/29 17:09:56 by mtrisha           #+#    #+#             */
/*   Updated: 2019/12/03 18:45:02 by mtrisha          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>

#include <lemin.h>

//TODO: int to __int32_t

int		main(void)
{
	ft_memman_init();
	lemin();
	ft_force_buff();
	ft_memman_clean();
	return (0);
}
