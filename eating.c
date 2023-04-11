/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eating.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbarreto <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/11 22:16:56 by mbarreto          #+#    #+#             */
/*   Updated: 2023/04/11 22:17:00 by mbarreto         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	lock_fork(t_data *data, t_table *table, int side)
{
	while (!check_dead(data, table))
	{
		pthread_mutex_lock(&table->data->forks[side]);
		if (!data->table[side].fork_lock)
		{
			data->table[side].fork_lock = 1;
			pthread_mutex_unlock(&table->data->forks[side]);
			break ;
		}
		else
			pthread_mutex_unlock(&table->data->forks[side]);
	}
	if (!death_var(data))
		printer(data, data->first_timestamp, table->id, "has taken a fork");
}

void	unlock_fork(t_data *data, t_table *table, int side)
{
	pthread_mutex_lock(&table->data->forks[side]);
	data->table[side].fork_lock = 0;
	pthread_mutex_unlock(&table->data->forks[side]);
}

int	philo_eat(t_data *data, t_table *table)
{
	int	left;
	int	right;

	left = ((table->id == 0) * (table->data->philo_num - 1)) + \
	((table->id != 0) * (table->id));
	right = (table->id - (table->id != 0));
	lock_fork(data, table, left);
	if (death_var(data))
		return (1);
	lock_fork(data, table, right);
	if (death_var(data))
		return (1);
	printer(data, data->first_timestamp, table->id, "is eating");
	table->x_ate++;
	table->last_meal_t = times();
	sleeping(data->eat_time, data);
	unlock_fork(data, table, left);
	unlock_fork(data, table, right);
	return (0);
}
