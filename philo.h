#ifndef PHILO_H
# define PHILO_H

#include <pthread.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/time.h>
#include <stdlib.h>
#include <string.h>

typedef struct s_data	t_data;

typedef struct s_table
{
	int				id;
	int				x_ate;
	int				left_fork;
	int				right_fork;
	long long		last_meal_t;
	pthread_t		thread_id;
	t_data			*data;
}	t_table;

typedef struct s_data
{
	int     philo_num;
	int     die_time;
	int     eat_time;
	int     sleep_time;
	int     eat_count;
	int     death;
	int		all_ate;
	pthread_mutex_t	writing;
	pthread_mutex_t	fork[250];
	pthread_mutex_t	eating;
	long long		first_timestamp;
	t_table			table[250];
}	t_data;


//init
int		ft_init(t_data *d, int ac, char **av);
int		initphil(t_data *d);
void	ft_checker(t_data *d, int ac);
int		initmut(t_data *d);

//utils
int	ft_atoi(const char *str);
long long	times(void);
long long	time_diff(long long past, long long pres);
void		sleeping(long long time, t_data *data);
void		printer(t_data *data, int id, char *string);
void	exit_launcher(t_data *data, t_table *table);

//working
int		execute(t_data *data);
void	check_dead(t_data *d, t_table *t);
void	*philo_thread(void *void_philosopher);
void	philo_eat(t_table *table);

//extra
long long	time_ms(t_data *d);
long long	time_2_eat(t_table *t);
void		thread_destroyer(t_data *d);

#endif
