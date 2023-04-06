
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbarreto <mbarreto@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/22 16:10:14 by mbarreto          #+#    #+#             */
/*   Updated: 2023/04/04 16:30:14 by mbarreto         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h" 

int checknum(char *arg)
{
	char	str[12] = "+-0123456789";
	size_t	i;
	size_t	j;

	i = -1;
	if (!arg)
		return (-1);
	while (arg[++i])
	{
		j = -1;
		while (str[++j])
		{
			if (arg[i] == str[j])
				break ;
		}
		if (arg[i] == str[j])
			continue ;
		return (-1);
	}
	return (0);
}

int	main(int ac, char **av)
{
	t_data	d;
	t_table	*table;
	int	i;

	i = ac - 1;
	if (ac < 5 || ac > 6)
	{
		printf("Wrong number of arguments\n");
		return (0);
	}
	while (i > 0)
	{
		if (checknum(av[i]) == -1)
		{
			printf("Wrong arguments, insert only numbers\n");
			return (0);
		}
		i--;
	}
	table = ft_init(&d, ac, av);
	if (table == 0)
		return (0);
	if (d.philo_num > 1)
		if (work(&d, table))
			return (printf("Error while creating the threads\n"));
	return (0);
}
