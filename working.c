/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   working.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbarreto <mbarreto@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/22 16:10:17 by mbarreto          #+#    #+#             */
/*   Updated: 2023/04/10 16:34:38 by mbarreto         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	lock_fork(t_data *data, t_table *table, int side)
{
	while (!check_dead(data, table))
	{
		pthread_mutex_lock(&table->data->forks[side]);
		if (!data->fork_lock)
		{
			data->fork_lock = 1;
			pthread_mutex_unlock(&table->data->forks[side]);
			break ;
		}
		else
			pthread_mutex_unlock(&table->data->forks[side]);
	}
	if (!check_dead(data, table))
		printer(data, data->first_timestamp, table->id, "has taken a fork");
}

void	unlock_fork(t_data *data, t_table *table, int side)
{
	pthread_mutex_lock(&table->data->forks[side]);
	data->fork_lock = 0;
	pthread_mutex_unlock(&table->data->forks[side]);
}

int	philo_eat(t_data *data, t_table *table)
{
	lock_fork(data, table, table->fork_left);
	printf("aquiiiiii\n");
	//printer(data, data->first_timestamp, table->id, "has taken a fork");
	if (check_dead(data, table))
		return (1);
	lock_fork(data, table, table->fork_right);
	//printer(data, data->first_timestamp, table->id, "has taken a fork");
	if (check_dead(data, table))
		return (1);
	printer(data, data->first_timestamp, table->id, "is eating");
	(table->x_ate)++;
	//pthread_mutex_lock(&(data->allate));
	table->last_meal_t = times();
	//pthread_mutex_unlock(&(data->allate));
	sleeping(data->eat_time, data);
	unlock_fork(data, table, table->fork_left);
	unlock_fork(data, table, table->fork_right);
	return (0);
}

void	*philo_thread(void *voidphil)
{
	t_table			*table;

	table = (t_table *)voidphil;
	if (table->id % 2)
		usleep(10000);
	while (!death_var(table->data))
	{
		if (philo_eat(table->data, table))
			return (NULL);
		if (table->data->all_ate)
			break ;
		printer(table->data, table->data->first_timestamp, table->id, "is sleeping");
		sleeping(table->data->sleep_time, table->data);
		printer(table->data, table->data->first_timestamp, table->id, "is thinking");
	}
	return (NULL);
}

int		death_var(t_data *d)
{
	int	death;

	pthread_mutex_lock(&(d->deathlock));
	death = d->death;
	pthread_mutex_unlock(&(d->deathlock));
	return (death);
}

int		check_dead(t_data *d, t_table *t)
{
	if (death_var(d))
		return (1);
	if (time_diff(t->last_meal_t, times()) >= d->die_time)
	{
		pthread_mutex_lock(&(d->deathlock));
		d->death++;
		if (d->death == 1)
			printer(d, t->last_meal_t, t->id, "died");
		pthread_mutex_unlock(&(d->deathlock));
		return (1);
	}
	return (0);
}

int	sleeping(long long time, t_data *data)
{
	long long	i;
	long long	j;

	i = times();
	j = 0;
	while (!(death_var(data)) && j < time)
	{
		if (death_var(data))
			return (1);
		usleep(100);
		j = time_diff(i, times());
	}
	return (j >= time);
}

int	work(t_data *data, t_table *table)
{
	int		i;

	i = -1;
	table->start = times();
	while (++i < data->philo_num)
	{
		if (pthread_create(&(table[i].thread_id), NULL, \
		philo_thread, (table + i)))
			return (1);
	}
	exit_launcher(data, table);
	free(table);
	return (0);
}
