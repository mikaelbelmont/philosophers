/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbarreto <mbarreto@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/22 16:10:16 by mbarreto          #+#    #+#             */
/*   Updated: 2023/04/12 16:38:53 by mbarreto         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	initmut(t_data *d)
{
	int	i;

	i = -1;
	d->forks = malloc(sizeof(t_data) * d->philo_num);
	while (++i <= d->philo_num)
	{
		if (pthread_mutex_init(&d->forks[i], NULL))
			return (1);
	}
	if (pthread_mutex_init(&d->writing, NULL))
		return (1);
	if (pthread_mutex_init(&d->allate, NULL))
		return (1);
	if (pthread_mutex_init(&d->deathlock, NULL))
		return (1);
	return (0);
}

int	ft_checker(t_data *d, int ac)
{
	int	i;

	i = 0;
	if (d->philo_num < 1)
	{
		printf("Insert 1 or more philosophers\n");
		return (-1);
	}
	i++;
	if (d->die_time > 0)
		i++;
	if (d->eat_time > 0)
		i++;
	if (d->sleep_time > 0)
		i++;
	if (ac == 6)
		if (d->eat_count > 0)
			i++;
	if ((ac == 5 && i == 4) || (ac == 6 && i == 5))
		return (1);
	printf("Incorrect arguments\n");
	return (-1);
}

t_table	*initphil(t_table *table, t_data *d)
{
	int	i;

	i = -1;
	table = malloc(sizeof(t_table) * d->philo_num);
	if (!table)
		return (NULL);
	d->first_timestamp = times();
	table->data = d;
	while (++i < d->philo_num)
	{
		table[i].id = i;
		table[i].x_ate = 0;
		table[i].last_meal_t = times();
		table[i].data = d;
		table[i].fork_lock = 0;
	}
	return (table);
}

void	*onephilo(void *tm_die)
{
	int	*tm_t_die;

	tm_t_die = (int *)tm_die;
	printf("%d %d has taken a fork\n", 0, 1);
	printf("%d %d died\n", *tm_t_die, 1);
	return (NULL);
}

t_table	*ft_init(t_data *d, int ac, char **av)
{
	t_table	*table;

	table = 0;
	d->philo_num = ft_atoi(av[1]);
	d->die_time = ft_atoi(av[2]);
	d->eat_time = ft_atoi(av[3]);
	d->sleep_time = ft_atoi(av[4]);
	if (d->philo_num == -1 || d->die_time == -1 || d->eat_time == -1 \
		|| d->sleep_time == -1)
	{
		printf("Wrong number of arguments\n");
		return (0);
	}
	d->death = 0;
	if (ac == 6)
		d->eat_count = ft_atoi(av[5]);
	else
		d->eat_count = -1;
	if (ft_checker(d, ac) == -1)
		return (NULL);
	initmut(d);
	table = initphil(table, d);
	d->table = table;
	return (table);
}
