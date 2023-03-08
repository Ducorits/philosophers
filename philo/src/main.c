/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   main.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: dritsema <dritsema@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/01/03 13:10:32 by dritsema      #+#    #+#                 */
/*   Updated: 2023/03/08 19:01:15 by dritsema      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <philo.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/time.h>

void	*time_thread(void *vargp)
{
	t_info			*info;
	struct timeval	stamp;
	struct timeval	start;

	gettimeofday(&start, NULL);
	info = (t_info *)vargp;
	while (1)
	{
		gettimeofday(&stamp, NULL);
		info->time_stamp = (((stamp.tv_sec * 1000000) + stamp.tv_usec)
				- ((start.tv_sec * 1000000) + start.tv_usec));
		usleep(1);
	}
	return (NULL);
}

void	end_sim(t_info *info)
{
	int	i;

	i = 0;
	while (i < info->philo_count)
	{
		pthread_mutex_destroy(&info->forks[i]);
		i++;
	}
	free(info->forks);
}

void	start_threads(t_info *info, pthread_t *id, t_philo *philos)
{
	pthread_t		time_id;
	pthread_t		monit_id;
	int				i;

	pthread_create(&time_id, NULL, time_thread, (void *)info);
	pthread_detach(time_id);
	i = 0;
	while (i < info->philo_count)
	{
		philos[i].id = i;
		philos[i].info = info;
		philos[i].last_meal = 0;
		philos[i].times_eaten = 0;
		pthread_create(&id[i], NULL, philo_thread, (void *)&philos[i]);
		i++;
	}
	pthread_create(&monit_id, NULL, monitor_thread, (void *)philos);
	pthread_detach(monit_id);
}

void	start_sim(t_info info)
{
	int				i;
	pthread_t		*id;
	t_philo			*philos;

	id = malloc(info.philo_count * sizeof(pthread_t));
	philos = malloc(info.philo_count * sizeof(t_philo));
	pthread_mutex_lock(&info.start);
	start_threads(&info, id, philos);
	pthread_mutex_unlock(&info.start);
	i = 0;
	while (i < info.philo_count)
	{
		pthread_join(id[i], NULL);
		i++;
	}
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
	t_info	info;

	if (argc > 4 && argc < 7)
	{
		info = init(argc, argv);
		start_sim(info);
		end_sim(&info);
	}
	else
	{
		printf("Usage: ./philosophers [philosopher count] ");
		printf("[time to die] [time to eat] [time to sleep]");
		printf(" {optional eat goal}\n");
	}
	return (0);
}
