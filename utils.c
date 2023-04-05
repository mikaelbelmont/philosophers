/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbarreto <mbarreto@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/22 16:10:12 by mbarreto          #+#    #+#             */
/*   Updated: 2023/04/05 00:01:05 by mbarreto         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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

void	printer(t_data *data, long long int timestamp, int id, char *string)
{
	printf("\n\n\neeeeeeeeeeeeee%lld\n\n\n", timestamp);
	pthread_mutex_lock(&(data->writing));
	printf("%lli ", (times() - timestamp));
	printf("%i ", id + 1);
	printf("%s\n", string);
	pthread_mutex_unlock(&(data->writing));
	return ;
}

void	exit_launcher(t_data *data, t_table *table)
{
	int	i;

	i = -1;
	while (++i <= data->philo_num)
		pthread_join(table[i].thread_id, NULL);
	i = -1;
	while (++i < data->philo_num)
		pthread_mutex_destroy(&(data->forks[i]));
	pthread_mutex_destroy(&(data->writing));
}
