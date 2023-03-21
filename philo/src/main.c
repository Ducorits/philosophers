/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   main.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: dritsema <dritsema@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/01/03 13:10:32 by dritsema      #+#    #+#                 */
/*   Updated: 2023/03/21 13:34:29 by dritsema      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <philo.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/time.h>

void	end_sim(t_info *info, pthread_t *id)
{
	int	i;

	i = 0;
	while (i < info->philo_count)
	{
		pthread_join(id[i], NULL);
		i++;
	}
	i = 0;
	while (i < info->philo_count)
	{
		pthread_mutex_destroy(&info->forks[i]);
		i++;
	}
	free(info->forks);
	free(info);
}

int	setup_philosophers(t_philo *philos, t_info *info, long start_time)
{
	int	i;

	i = 0;
	while (i < info->philo_count)
	{
		philos[i].id = i;
		philos[i].info = info;
		philos[i].last_meal = 0;
		philos[i].times_eaten = 0;
		philos[i].state = THINKING;
		philos[i].start_time = start_time;
		if (info->philo_count % 2)
			choose_fork_uneven(&philos[i], i);
		else
			choose_fork_even(&philos[i], i);
		if (pthread_mutex_init(&philos[i].state_lock, NULL)
			|| pthread_mutex_init(&philos[i].eat_lock, NULL)
			|| pthread_mutex_init(&philos[i].time_lock, NULL))
			return (1);
		i++;
	}
	return (0);
}

int	start_threads(t_info *info, pthread_t *id, t_philo *philos)
{
	pthread_t		monit_id;
	struct timeval	time;
	long			start_time;
	int				i;

	gettimeofday(&time, NULL);
	start_time = (((time.tv_sec * 1000000) + time.tv_usec));
	if (setup_philosophers(philos, info, start_time))
		return (1);
	i = 0;
	while (i < info->philo_count)
	{
		if (pthread_create(&id[i], NULL, philo_thread, (void *)&philos[i]))
			return (1);
		i++;
	}
	if (pthread_create(&monit_id, NULL, monitor_thread, (void *)philos))
		return (1);
	if (pthread_detach(monit_id))
		return (1);
	return (0);
}

void	start_sim(t_info *info)
{
	// int				i;
	pthread_t		*id;
	t_philo			*philos;

	id = malloc(info->philo_count * sizeof(pthread_t));
	philos = malloc(info->philo_count * sizeof(t_philo));
	if (philos == NULL || id == NULL)
		return (free(philos), free(id));
	pthread_mutex_lock(&info->start);
	if (start_threads(info, id, philos))
	{
		pthread_mutex_unlock(&info->start);
		return (end_sim(info, id));
	}
	pthread_mutex_unlock(&info->start);
	end_sim(info, id);
	free(id);
	free(philos);
	return ;
}

// arguments:
// philo count,
// time to die,
// time to eat,
// time to sleep,
// optional eat goal.
int	main(int argc, char **argv)
{
	t_info	*info;

	if (argc > 4 && argc < 7)
	{
		info = init(argc, argv);
		if (info == NULL)
		{
			printf("Invalid arguments or error :)\n");
			return (1);
		}
		start_sim(info);
	}
	else
	{
		printf("Usage: ./philosophers [philosopher count] ");
		printf("[time to die] [time to eat] [time to sleep]");
		printf(" {optional eat goal}\n");
	}
	return (0);
}
