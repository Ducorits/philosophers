/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   start_threads.c                                    :+:    :+:            */
/*                                                     +:+                    */
/*   By: dritsema <dritsema@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/03/27 15:00:34 by dritsema      #+#    #+#                 */
/*   Updated: 2023/04/03 15:36:40 by dritsema      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <philo.h>

int	setup_mutexes(t_philo *philo)
{
	if (pthread_mutex_init(&philo->state_lock, NULL))
		return (1);
	if (pthread_mutex_init(&philo->eat_lock, NULL))
	{
		pthread_mutex_destroy(&philo->state_lock);
		return (1);
	}
	if (pthread_mutex_init(&philo->time_lock, NULL))
	{
		pthread_mutex_destroy(&philo->state_lock);
		pthread_mutex_destroy(&philo->eat_lock);
		return (1);
	}
	return (0);
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
		philos[i].state = ALIVE;
		philos[i].start_time = start_time;
		if (info->philo_count % 2)
			choose_fork_uneven(&philos[i], i);
		else
			choose_fork_even(&philos[i], i);
		if (setup_mutexes(&philos[i]))
			return (clear_philos(philos, i - 1), 1);
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
			return (join_threads(id, i - 1), 1);
		i++;
	}
	if (pthread_create(&monit_id, NULL, monitor_thread, (void *)philos)
		|| pthread_detach(monit_id))
		return (1);
	return (0);
}
