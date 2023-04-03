/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   working.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbarreto <mbarreto@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/22 16:10:17 by mbarreto          #+#    #+#             */
/*   Updated: 2023/04/03 17:46:43 by mbarreto         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	take_fork(t_table *table, int side)
{
	pthread_mutex_lock(&table->data.forks[side]);
	printer(&table->data, table->id, "has taken a fork");
	pthread_mutex_unlock(&table->data.forks[side]);
}

void	philo_eat(t_table *table)
{
	t_data	data;

	data = table->data;
	take_fork(table, data.fork_left);
	// pthread_mutex_lock(&data.forks[0]);
	// printer(&data, table->id, "has taken a fork");
	// pthread_mutex_lock(&data.forks[1]);
	// printer(&data, table->id, "has taken a fork");
	//pthread_mutex_lock(&(data->eating));
	take_fork(table, data.fork_right);
	printer(&data, table->id, "is eating");
	table->last_meal_t = times();
	//pthread_mutex_unlock(&(data->eating));
	pthread_mutex_lock(&(data.util));
	(table->x_ate)++;
	pthread_mutex_unlock(&(data.util));
	sleeping(data.eat_time, &data);
	//pthread_mutex_unlock(data.fork_left);
	//pthread_mutex_unlock(data.fork_right);
}

void	*philo_thread(void *voidphil)
{
	int				i;
	t_table			*table;
	t_data			data;

	i = 0;
	table = (t_table *)voidphil;
	data = table->data;
	if (table->id % 2)
		usleep(1000);
	//pthread_mutex_lock(&(data->deathlock));
	while (!(data.death))
	{
		philo_eat(table);
		pthread_mutex_lock(&(data.allate));
		if (data.all_ate)
			break ;
		pthread_mutex_unlock(&(data.allate));
		printer(&data, table->id, "is sleeping");
		sleeping(data.sleep_time, &data);
		printer(&data, table->id, "is thinking");
		i++;
	}
	//pthread_mutex_unlock(&(data->deathlock));
	return (NULL);
}

void	check_dead(t_data *d, t_table *t)
{
	int	i;
	int	death;
	
	pthread_mutex_lock(&(d->util2));
	death = d->death;
	pthread_mutex_unlock(&(d->util2));
	while (!(d->all_ate))
	{
		i = -1;
		while (++i < d->philo_num && !(death))
		{
			//pthread_mutex_lock(&(d->eating));
			if (time_diff(t[i].last_meal_t, times()) > d->die_time)
			{
				printer(d, i, "died");
				death = 1;
			}
			//pthread_mutex_unlock(&(d->eating));
			usleep(100);
		}
		if (death)
		{
			pthread_mutex_lock(&(d->util2));
			d->death = 1;
			pthread_mutex_unlock(&(d->util2));
			return ;
		}

		i = 0;
		//pthread_mutex_lock(&(d->util));
		while (d->eat_count != -1 && i < d->philo_num && \
		t[i].x_ate >= (d->eat_count - 1))
			i++;
		//pthread_mutex_unlock(&(d->util));
		//pthread_mutex_lock(&(d->allate));
		if (i == d->philo_num)
			d->all_ate = 1;
		//pthread_mutex_unlock(&(d->allate));
	}
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

int	work(t_data *data)
{
	int		i;
	t_table	*table;

	i = -1;
	table = malloc(sizeof(t_table));
	data->first_timestamp = times();
	while (++i < data->philo_num)
	{
		printf("%d\n", i);
		if (pthread_create(&(table[i].thread_id), NULL, \
		philo_thread, (table + i)))
			return (1);
		pthread_mutex_lock(&(data->eating));
		table[i].last_meal_t = times();
		pthread_mutex_unlock(&(data->eating));
	}
	check_dead(data, table);
	exit_launcher(data, table);
	free(table);
	return (0);
}
