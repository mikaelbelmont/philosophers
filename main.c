#include "philo.h"

int	main(int ac, char **av)
{
	t_data  d;

	if (ac < 5 || ac > 6)
	{
		printf("Wrong number of arguments\n");
		exit(0);
	}
	ft_init(&d, ac, av);
	if (execute(&d))
		return(printf("Error while creating the threads\n"));
	return (0);
}
