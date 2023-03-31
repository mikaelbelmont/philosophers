// #include "philo.h"

// long long	time_ms(t_data *d)
// {
// 	struct timeval  tv;
// 	long long		s;
// 	long long		us;

// 	gettimeofday(&tv, 0);
// 	s = tv.tv_sec - d->s_time.tv_sec;
// 	us = tv.tv_usec - d->s_time.tv_usec;
// 	return ((us / 1000) + (s * 1000));
// }

// long long	time_2_eat(t_table *t)
// {
// 	struct timeval  tv;
// 	long long s;
// 	long long us;

// 	gettimeofday(&tv, 0);
// 	s = tv.tv_sec - t->time.tv_sec;
// 	us = tv.tv_usec - t->time.tv_usec;
// 	return ((us / 1000) + (s * 1000));
// }

// void thread_destroyer(t_data *d)
// {
// 	int	i;

// 	i = -1;
// 	while (++i < d->philo_num)
// 		pthread_join(d->table[i].table, NULL);
// 	pthread_mutex_destroy(&d->isdying);
// 	pthread_mutex_destroy(&d->eating);
// 	i = -1;
// 	while(++i < d->philo_num)
// 		pthread_mutex_destroy(&d->table[i].fork);
// 	free (d->table);
// }
