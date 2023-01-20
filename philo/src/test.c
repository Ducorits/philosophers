/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   test.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: dritsema <dritsema@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/01/03 13:10:32 by dritsema      #+#    #+#                 */
/*   Updated: 2023/01/05 17:23:41 by dritsema      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <philo.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

static int	ft_atoi(const char *str)
{
	unsigned int	n;
	int				mins;

	n = 0;
	mins = 1;
	while ((*str >= 9 && *str <= 13) || *str == ' ')
		str++;
	if (*str == '-')
		mins = -1;
	if (*str == '-' || *str == '+')
		str++;
	while (*str >= '0' && *str <= '9')
	{
		n = n * 10 + (*str - '0');
		str++;
	}
	return ((int)(n * mins));
}

t_philo	*init_philosophers(t_info info)
{
	t_philo	*philos;
	int		i;

	i = 0;
	philos = malloc(info.philo_count * sizeof(t_philo));
	while (i < info.philo_count)
	{
		philos[i].state = IDLE;
		philos[i].id = i;
		i++;
	}
	return (philos);
}

t_info	init(int argc, char **argv)
{
	t_info	info;

	info.philo_count = ft_atoi(argv[1]);
	info.time_to_die = ft_atoi(argv[2]);
	info.time_to_eat = ft_atoi(argv[3]);
	info.time_to_sleep = ft_atoi(argv[4]);
	if (argc == 6)
		info.eat_goal = ft_atoi(argv[5]);
	else
		info.eat_goal = 0;
	info.philos = init_philosophers(info);
	return (info);
}

void	*philo_thread(void *vargp)
{
	t_philo	*philo;
	int		intid;

	philo = (t_philo *)vargp;
	intid = philo->id;
	// sleep(1);
	printf("test in thread id: %i\n", intid);
	return (NULL);
}

int	main(int argc, char **argv)
{
	t_info		info;
	pthread_t	id;
	int			i;

	if (argc > 4 && argc < 7)
	{
		i = 0;
		info = init(argc, argv);
		while (i < info.philo_count)
		{
			pthread_create(&id, NULL, philo_thread, (void *)&info.philos[i]);
			// pthread_join(id, NULL);
			i++;
		}
		sleep(1);
	}
	// while (1)
	return (0);
}
