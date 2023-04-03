/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   working.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbarreto <mbarreto@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/22 16:10:17 by mbarreto          #+#    #+#             */
/*   Updated: 2023/04/02 17:21:35 by mbarreto         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	philo_eat(t_table *table)
{
	t_data	data;

	data = table->data;
	pthread_mutex_lock(data.fork_left);
	printer(&data, table->id, "has taken a fork");
	pthread_mutex_lock(data.fork_right);
	printer(&data, table->id, "has taken a fork");
	//pthread_mutex_lock(&(data->eating));
	printer(&data, table->id, "is eating");
	table->last_meal_t = times();
	//pthread_mutex_unlock(&(data->eating));
	pthread_mutex_lock(&(data.util));
	(table->x_ate)++;
	pthread_mutex_unlock(&(data.util));
	sleeping(data.eat_time, &data);
	pthread_mutex_unlock(data.fork_left);
	pthread_mutex_unlock(data.fork_right);
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

void	*onephilo(void *tm_die)
{
	int	*tm_t_die;

	tm_t_die = (int *)tm_die;
	printf("%d %d has taken a fork\n", 0, 1);
	printf("%d %d died\n", *tm_t_die, 1);
	return (NULL);
}

int	work(t_data *data)
{
	int		i;
	t_table	*table;

	i = -1;
	table = 0;
	data->first_timestamp = times();
	while (++i < data->philo_num)
	{
		if (pthread_create(&(table[i].thread_id), NULL, \
		philo_thread, (table + i) ))
			return (1);
		pthread_mutex_lock(&(data->eating));
		table[i].last_meal_t = times();
		pthread_mutex_unlock(&(data->eating));
	}
	check_dead(data, table);
	exit_launcher(data, table);
	return (0);
}
