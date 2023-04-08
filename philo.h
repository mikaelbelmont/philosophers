/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbarreto <mbarreto@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/22 16:10:09 by mbarreto          #+#    #+#             */
/*   Updated: 2023/04/07 19:55:19 by mbarreto         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <pthread.h>
# include <unistd.h>
# include <stdio.h>
# include <sys/time.h>
# include <stdlib.h>
# include <string.h>

typedef struct s_data	t_data;

struct s_data
{
	int				philo_num;
	int				die_time;
	int				eat_time;
	int				sleep_time;
	int				eat_count;
	int				death;
	int				all_ate;
	pthread_mutex_t	writing;
	
	pthread_mutex_t	*forks;
	
	// pthread_mutex_t	*fork_left;
	// pthread_mutex_t	*fork_right;
	
	int				fork_left;
	int				fork_right;

	pthread_mutex_t	eating;
	pthread_mutex_t	util;
	pthread_mutex_t	allate;
	pthread_mutex_t	util2;
	pthread_mutex_t	deathlock;
	long long		first_timestamp;
};

typedef struct s_table
{
	int				id;
	int				x_ate;
	// int				left_fork;
	// int				right_fork;
	long long		last_meal_t;
	pthread_t		thread_id;
	t_data			*data;

	long long	start;

	//t_table		**mutex;
	
}	t_table;

//init
int			initmut(t_data *d);
int			ft_checker(t_data *d, int ac);
t_table		*initphil(t_table *table, t_data *d);
void		*onephilo(void *tm_die);
t_table		*ft_init(t_data *d, int ac, char **av);

//utils
int			ft_atoi(const char *str);
long long	times(void);
long long	time_diff(long long past, long long pres);
void		printer(t_data *data, long long int timestamp, int id, char *string);
void		exit_launcher(t_data *data, t_table *table);

//working
int			philo_eat(t_data *data, t_table *table);
void		*philo_thread(void *voidphil);
int			check_dead(t_data *d, t_table *t);
int		sleeping(long long time, t_data *data);
int			work(t_data *data, t_table *table);

#endif
