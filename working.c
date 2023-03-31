#include "philo.h"

void	philo_eat(t_table *table)
{
	t_data *data;

	data = table->data;
	pthread_mutex_lock(&(data->fork[table->left_fork]));
	printer(data, table->id, "has taken a fork");
	pthread_mutex_lock(&(data->fork[table->right_fork]));
	printer(data, table->id, "has taken a fork");
	pthread_mutex_lock(&(data->eating));
	printer(data, table->id, "is eating");
	table->last_meal_t = times();
	pthread_mutex_unlock(&(data->eating));
	sleeping(data->eat_time, data);
	(table->x_ate)++;
	pthread_mutex_unlock(&(data->fork[table->left_fork]));
	pthread_mutex_unlock(&(data->fork[table->right_fork]));
}

void	*philo_thread(void *void_philosopher)
{
	int				i;
	t_table			*table;
	t_data			*data;

	i = 0;
	table = (t_table *)void_philosopher;
	data = table->data;
	if (table->id % 2)
		usleep(15000);
	while (!(data->death))
	{
		philo_eat(table);
		if (data->all_ate)
			break ;
		printer(data, table->id, "is sleeping");
		sleeping(data->sleep_time, data);
		printer(data, table->id, "is thinking");
		i++;
	}
	return (NULL);
}

void	check_dead(t_data *d, t_table *t)
{
	int i;

	while (!(d->all_ate))
	{
		i = -1;
		while (++i < d->philo_num && !(d->death))
		{
			pthread_mutex_lock(&(d->eating));
			if (time_diff(t[i].last_meal_t, times()) > d->die_time)
			{
				printer(d, i, "died");
				d->death = 1;
			}
			pthread_mutex_unlock(&(d->eating));
			usleep(100);
		}
		if (d->death)
			break;
		i = 0;
		while (d->eat_count != -1 && i < d->philo_num && t[i].x_ate >= d->eat_count)
			i++;
		if (i == d->philo_num)
			d->all_ate = 1;
	}
}

int		execute(t_data *data)
{
	int				i;
	t_table	*table;

	i = 0;
	table = data->table;
	data->first_timestamp = times();
	while (i < data->philo_num)
	{
		if (pthread_create(&(table[i].thread_id), NULL, philo_thread, &(table[i])))
			return (1);
		table[i].last_meal_t = times();
		i++;
	}
	check_dead(data, data->table);
	exit_launcher(data, table);
	return (0);
}