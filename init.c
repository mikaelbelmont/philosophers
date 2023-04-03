/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbarreto <mbarreto@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/22 16:10:16 by mbarreto          #+#    #+#             */
/*   Updated: 2023/04/03 17:45:27 by mbarreto         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	initmut(t_data *d)
{
	int	i;

	i = -1;    
	d->forks = malloc(sizeof(d) * d->philo_num);
	while (++i >= d->philo_num)
	{
		if (pthread_mutex_init(&d->forks[i], NULL))
			return (1);
	}
	if (pthread_mutex_init(&d->writing, NULL))
		return (1);
	if (pthread_mutex_init(&d->eating, NULL))
		return (1);
	if (pthread_mutex_init(&d->util, NULL))
		return (1);
	if (pthread_mutex_init(&d->allate, NULL))
		return (1);
	if (pthread_mutex_init(&d->util2, NULL))
		return (1);
	if (pthread_mutex_init(&d->deathlock, NULL))
		return (1);
	// while (++i >= d->philo_num)
	// {
	// 	d[i].fork_left = &d->forks[i + 1];	
	// }
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
	if (d->die_time >= 0)
		i++;
	if (d->eat_time >= 0)
		i++;
	if (d->sleep_time >= 0)
		i++;
	if (ac == 6)
		if (d->eat_count >= 0)
			i++;
	if ((ac == 5 && i == 4) || (ac == 6 && i == 5))
		return (1);
	printf("Incorrect arguments\n");
	return (-1);
}

int	initphil(t_table *table, t_data *d)
{
	int i;
	
	i = d->philo_num;
	table = malloc(sizeof(t_table) * i);
	if (!table)
		return (-1);
	while (--i >= 0)
	{
		table[i].id = i;
		table[i].x_ate = 0;
		// if ((i + 1) != d->philo_num)
		// {
		// 	table[i].right_fork = (i + 1) % d->philo_num;
		// 	table[i].left_fork = i;
		// }
		// else
		// {
		// 	table[i].left_fork = (i + 1) % d->philo_num;
		// 	table[i].right_fork = i;
		// }
		table->left_fork = i;
		table->right_fork = (i + 1) % d->philo_num;
		table[i].last_meal_t = 0;
		table[i].data = *d;
	}
	return (0);
}

void	*onephilo(void *tm_die)
{
	int	*tm_t_die;

	tm_t_die = (int *)tm_die;
	printf("%d %d has taken a fork\n", 0, 1);
	printf("%d %d died\n", *tm_t_die, 1);
	return (NULL);
}

int	ft_init(t_data *d, int ac, char **av)
{
	t_table	*table;

	table = 0;
	d->philo_num = ft_atoi(av[1]);
	d->die_time = ft_atoi(av[2]);
	d->eat_time = ft_atoi(av[3]);
	d->sleep_time = ft_atoi(av[4]);
	d->death = 0;
	d->all_ate = 0;
	if (ac == 6)
		d->eat_count = ft_atoi(av[5]);
	else
		d->eat_count = -1;
	if (d->philo_num == 1)
	{
		table = malloc(sizeof(t_table));
		pthread_create(&(table[0].thread_id), NULL, onephilo, &(d->die_time));
		pthread_join(table[0].thread_id, NULL);
		usleep(500);
		return (-1);
	}
	if (ft_checker(d, ac) == -1)
		return (-1);
	initmut(d);
	initphil(table, d);
	free(table);
	return (0);
}
