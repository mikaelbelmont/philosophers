#include "philo.h"

int	ft_atoi(const char *str)
{
	int	i;
	int	n;
	int	neg;

	i = 0;
	n = 0;
	neg = 1;
	while (str[i] == ' ' || str[i] == '\n' || str[i] == '\t'
		|| str[i] == '\v' || str[i] == '\f' || str[i] == '\r')
		i++;
	if (str[i] == '-' || str[i] == '+' || str[i] < '0' 
		|| str[i] > '9')
		return (-1);
	while (str[i] >= '0' && str[i] <= '9')
	{
		n = n * 10 + (str[i] - '0');
		i++;
	}
	return (n * neg);
}

long long	times(void)
{
	struct timeval	t;

	gettimeofday(&t, NULL);
	return ((t.tv_sec * 1000) + (t.tv_usec / 1000));
}

long long	time_diff(long long past, long long pres)
{
	return (pres - past);
}

void		sleeping(long long time, t_data *data)
{
	long long i;

	i = times();
	while (!(data->death))
	{
		if (time_diff(i, times()) >= time)
			break ;
		usleep(50);
	}
}

void		printer(t_data *data, int id, char *string)
{
	pthread_mutex_lock(&(data->writing));
	if (!(data->death))
	{
		printf("%lli ", times() - data->first_timestamp);
		printf("%i ", id + 1);
		printf("%s\n", string);
	}
	pthread_mutex_unlock(&(data->writing));
	return ;
}

void	exit_launcher(t_data *data, t_table *table)
{
	int i;

	i = 0;
	while (++i <= data->philo_num)
		pthread_join(table[i].thread_id, NULL);
	i = -1;
	while (++i < data->philo_num)
		pthread_mutex_destroy(&(data->fork[i]));
	pthread_mutex_destroy(&(data->writing));
}