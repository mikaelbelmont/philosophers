/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   working.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbarreto <mbarreto@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/22 16:10:17 by mbarreto          #+#    #+#             */
/*   Updated: 2023/04/05 00:00:51 by mbarreto         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	philo_eat(t_table *table)
{
	t_data	data;

	data = table->data;
	pthread_mutex_lock(&table->data.forks[data.fork_left]);
	printer(&data, data.first_timestamp, table->id, "has taken a fork");
	pthread_mutex_unlock(&table->data.forks[data.fork_left]);
	pthread_mutex_lock(&table->data.forks[data.fork_right]);
	printer(&data, data.first_timestamp, table->id, "has taken a fork");
	pthread_mutex_unlock(&table->data.forks[data.fork_right]);
	printer(&data, data.first_timestamp, table->id, "is eating");
	//table->last_meal_t = times();
	pthread_mutex_lock(&(data.util));
	(table->x_ate)++;
	pthread_mutex_unlock(&(data.util));
	sleeping(data.eat_time, &data);
}

void	*philo_thread(void *voidphil)
{
	t_table			*table;
	t_data			data;

	table = (t_table *)voidphil;
	data = table->data;
	if (table->id % 2)
		usleep(10000);
	while (!(data.death))
	{
		philo_eat(table);
		if (data.all_ate)
			break ;
		printer(&data, data.first_timestamp, table->id, "is sleeping");
		sleeping(data.sleep_time, &data);
		printer(&data, data.first_timestamp, table->id, "is thinking");
	}
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
			//printf("\n\n\n%lld\n\n\n", d->first_timestamp);
			if (time_diff(t[i].last_meal_t, times()) > d->die_time)
			{
				printf("\n\n\n%lld\n\n\n", d->first_timestamp);
				printer(d, d->first_timestamp, i, "died");
				death = 1;
			}
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
		while (d->eat_count != -1 && i < d->philo_num && \
		t[i].x_ate >= (d->eat_count - 1))
			i++;
		if (i == d->philo_num)
			d->all_ate = 1;
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

int	work(t_data *data, t_table *table)
{
	int		i;

	i = -1;
	data->first_timestamp = times();
	printf("\n%lld\n", data->first_timestamp);
	while (++i < data->philo_num)
	{
		//pthread_mutex_lock(&(data->eating));
		table[i].last_meal_t = 0;
		//pthread_mutex_unlock(&(data->eating));
		if (pthread_create(&(table[i].thread_id), NULL, \
		philo_thread, (table + i)))
			return (1);
	}
	check_dead(data, table);
	exit_launcher(data, table);
	free(table);
	return (0);
}
