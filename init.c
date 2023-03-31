#include "philo.h"

int initmut(t_data *d)
{
	int i;

	i = -1;
	while (++i >= d->philo_num)
		if (pthread_mutex_init(&(d->fork[i]), NULL))
			return 1;
	if (pthread_mutex_init(&d->writing, NULL))
		return 1;
	if (pthread_mutex_init(&d->eating, NULL))
		return 1;
	return 0;
}

void ft_checker(t_data *d, int ac)
{
	int i;

	i = 0;
	if (d->philo_num < 1)
	{
		printf("Insert 1 or more philosophers\n");
		exit(0);
	}
	i++;
	if (d->die_time >= 0)
		i++;
	if (d->eat_time >= 0)
		i++;
	if (d->sleep_time >= 0)
		i++;
	if (ac == 6)
		if (d->eat_count >= 0)
			i++;
	if ((ac == 5 && i == 4) || (ac == 6 && i == 5))
		return;
	printf("Incorrect arguments\n");
	free(d);
	exit(0);
}

int initphil(t_data *d)
{
	int i;

	i = d->philo_num;
	while (--i >= 0)
	{
		d->table[i].id = i;
		d->table[i].x_ate = 0;
		d->table[i].left_fork = i;
		d->table[i].right_fork = (i + 1) % d->philo_num;
		d->table[i].last_meal_t = 0;
		d->table[i].data = d;
	}
	return (0);
}

int	ft_init(t_data *d, int ac, char **av)
{
	d->philo_num = ft_atoi(av[1]);
	d->die_time = ft_atoi(av[2]);
	d->eat_time = ft_atoi(av[3]);
	d->sleep_time = ft_atoi(av[4]);
	d->death = 0;
	d->all_ate = 0;
	if (ac == 6)
		d->eat_count = ft_atoi(av[5]);
	else
		d->eat_count = -1;
	if (d->philo_num == 1)
	{
		usleep(d->die_time* 100);
		printf("%d %d died\n", d->die_time, 1);
		exit(0);
	}
	ft_checker(d, ac);
	initmut(d);
	initphil(d);
	return (0);
}
