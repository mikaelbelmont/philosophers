/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   working.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbarreto <mbarreto@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/22 16:10:17 by mbarreto          #+#    #+#             */
/*   Updated: 2023/04/06 19:39:27 by mbarreto         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	philo_eat(t_data *data, t_table *table)
{
	// t_data	data;

	// data = table->data;
	pthread_mutex_lock(&table->data->forks[data->fork_left]);
	printer(data, data->first_timestamp, table->id, "has taken a fork");
	if (check_dead(data, table))
		return (1);
	pthread_mutex_lock(&table->data->forks[data->fork_right]);
	printer(data, data->first_timestamp, table->id, "has taken a fork");
	if (check_dead(data, table))
		return (1);
	printer(data, data->first_timestamp, table->id, "is eating");
	(table->x_ate)++;
	pthread_mutex_lock(&(data->allate));
	table->last_meal_t = times();
	pthread_mutex_unlock(&(data->allate));
	sleeping(data->eat_time, data);
	pthread_mutex_unlock(&table->data->forks[data->fork_right]);
	pthread_mutex_unlock(&table->data->forks[data->fork_left]);
	return (0);
}

void	*philo_thread(void *voidphil)
{
	t_table			*table;

	table = (t_table *)voidphil;
	if (table->id % 2)
		usleep(10000);
	//table->last_meal_t = times();
	while (!check_dead(table->data, table))
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

int		check_dead(t_data *d, t_table *t)
{
	int	death;
	long long ts;
	
	death = d->death;
	pthread_mutex_lock(&(d->allate));
	ts = t->last_meal_t;
	pthread_mutex_unlock(&(d->allate));
	if (death)
		return (1);
	//printf("%lld %lld - %lld > %d\n", t->last_meal_t, times(), t->start, d->die_time);
	//printf("\n\n%lld\n\n\n", time_diff(ts, (times() - t->start)));
	if (time_diff(ts, times()) >= d->die_time)
	{
		pthread_mutex_lock(&(d->deathlock));
		d->death = 1;
		if (d->death == 1)
			printer(d, t->last_meal_t, t->id, "died");
		pthread_mutex_unlock(&(d->deathlock));
		return (1);
	}
	return (0);
}

void	sleeping(long long time, t_data *data)
{
	long long	i;
	int			death;

	pthread_mutex_lock(&(data->util2));
	death = data->death;
	pthread_mutex_unlock(&(data->util2));
	i = times();
	while (!(death))
	{
		if (time_diff(i, times()) >= time)
			break ;
		usleep(50);
	}
}

int	work(t_data *data, t_table *table)
{
	int		i;

	i = -1;
	//data->first_timestamp = times();
	table->start = times();
	while (++i < data->philo_num)
	{
		//printf("aquiiiiii\n");
		//pthread_mutex_lock(&(data->util2));
		//table->last_meal_t = times() - table->start;
		//pthread_mutex_unlock(&(data->util2));
		if (pthread_create(&(table[i].thread_id), NULL, \
		philo_thread, (table + i)))
			return (1);
	}
	while (!check_dead(data, table))
		usleep(500);
	exit_launcher(data, table);
	free(table);
	return (0);
}
