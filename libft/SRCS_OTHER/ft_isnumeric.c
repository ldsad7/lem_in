/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isnumeric.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsprigga <bsprigga@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/26 18:01:26 by bsprigga          #+#    #+#             */
/*   Updated: 2019/03/02 19:00:15 by tsimonis         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

int	ft_isnumeric(char *arr)
{
	while (*arr)
	{
		if (*arr < '0' || *arr > '9')
			return (0);
		arr++;
	}
	return (1);
}