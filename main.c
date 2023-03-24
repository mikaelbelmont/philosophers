/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbarreto <mbarreto@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/22 16:10:14 by mbarreto          #+#    #+#             */
/*   Updated: 2023/03/24 16:13:31 by mbarreto         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int checknum(char *str)
{
	printf("%s\n", str);
	while (*str)
	{
		printf("entrou\n");
		while ((*str >= '0' && *str <= '9') || *str == '+' \
		|| *str == '-' || *str == ' ' || *str == '\t')
			str++;
		if (*str == '\0')
			return (1);
	}
	return (-1);
}

int	main(int ac, char **av)
{
	t_data	d;
	//int i;

	//i = 0;
	if (ac < 5 || ac > 6)
	{
		printf("Wrong number of arguments\n");
		exit(0);
	}
	//while (av[i])
	//{
	//	if (checknum(av[i]) == -1)
	//	{
	//		printf("Wrong arguments, insert only numbers\n");
	//		return (0);
	//	}
	//	i++;
	//}
	if (ft_init(&d, ac, av) == -1)
		return (0);
	if (d.philo_num > 1)
		if (work(&d))
			return (printf("Error while creating the threads\n"));
	return (0);
}
