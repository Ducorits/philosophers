/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   main.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: dritsema <dritsema@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/01/03 13:10:32 by dritsema      #+#    #+#                 */
/*   Updated: 2023/03/13 16:53:51 by dritsema      ########   odam.nl         */
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
	free(info);
}

int	start_threads(t_info *info, pthread_t *id, t_philo *philos)
{
	pthread_t		time_id;
	pthread_t		monit_id;
	int				i;

	if (pthread_create(&time_id, NULL, time_thread, (void *)info))
		return (1);
	if (pthread_detach(time_id))
		return (1);
	i = 0;
	while (i < info->philo_count)
	{
		philos[i].id = i;
		philos[i].info = info;
		philos[i].last_meal = 0;
		philos[i].times_eaten = 0;
		if (pthread_mutex_init(&philos[i].lock, NULL))
			return (1);
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
	int				i;
	pthread_t		*id;
	t_philo			*philos;

	id = malloc(info->philo_count * sizeof(pthread_t));
	philos = malloc(info->philo_count * sizeof(t_philo));
	if (philos == NULL || id == NULL)
		return (free(philos), free(id));
	pthread_mutex_lock(&info->start);
	if (start_threads(info, id, philos))
	{
		pthread_mutex_lock(&info->lock);
		info->sim_end = 1;
		pthread_mutex_unlock(&info->lock);
	}
	pthread_mutex_unlock(&info->start);
	i = 0;
	while (i < info->philo_count)
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
	t_info	*info;

	if (argc > 4 && argc < 7)
	{
		printf("init\n");
		info = init(argc, argv);
		if (info == NULL)
			return (1);
		printf("start-sim\n");
		start_sim(info);
		printf("end sim\n");
		end_sim(info);
	}
	else
	{
		printf("Usage: ./philosophers [philosopher count] ");
		printf("[time to die] [time to eat] [time to sleep]");
		printf(" {optional eat goal}\n");
	}
	return (0);
}
