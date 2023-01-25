/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   main.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: dritsema <dritsema@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/01/03 13:10:32 by dritsema      #+#    #+#                 */
/*   Updated: 2023/01/25 16:55:40 by dritsema      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <philo.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/time.h>

void	*monitor_thread(void *vargp)
{
	return (NULL);
}

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

void	start_sim(t_info info)
{
	struct timeval	start;
	int				i;
	pthread_t		*id;
	t_philo			*philos;
	pthread_t		time_id;

	i = 0;
	gettimeofday(&start, NULL);
	id = malloc(info.philo_count * sizeof(pthread_t));
	philos = malloc(info.philo_count * sizeof(t_philo));
	pthread_create(&time_id, NULL, time_thread, (void *)&info);
	pthread_detach(time_id);
	while (i < info.philo_count)
	{
		philos[i].id = i;
		philos[i].info = &info;
		philos[i].state = IDLE;
		philos[i].last_meal = ((start.tv_sec * 1000000) + start.tv_usec) / 1000;
		philos[i].times_eaten = 0;
		pthread_create(&id[i], NULL, philo_thread, (void *)&philos[i]);
		i++;
	}
	i = 0;
	while (i < info.philo_count)
	{
		pthread_join(id[i], NULL);
		i++;
	}
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
		printf("init\n");
		info = init(argc, argv);
		printf("start_sim\n");
		start_sim(info);
		printf("end_sim\n");
		end_sim(&info);
	}
	return (0);
}
