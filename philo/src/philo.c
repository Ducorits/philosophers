/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   philo.c                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: dritsema <dritsema@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/01/11 20:07:04 by dritsema      #+#    #+#                 */
/*   Updated: 2023/01/13 19:12:17 by dritsema      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <philo.h>
#include <stdio.h>
#include <sys/time.h>
#include <unistd.h>

int	try_to_eat(t_philo *philo)
{
	int	left;

	if (philo->id != 0)
		left = id - 1;
	else
		left = info->philo_count - 1;
	if (!pthread_mutex_lock(&info->forks[left]) && !pthread_mutex_lock(&info->forks[id]))
	{
		if (info->time_stamp - philo->last_meal
			> info->time_to_die * 1000)
		{
			pthread_mutex_unlock(&info->forks[id]);
			pthread_mutex_unlock(&info->forks[left]);
			return (0);
		}
		info->previous_stamp = info->time_stamp;
		printf("%ld %i has taken a fork\n", info->time_stamp / 1000, id);
		printf("%ld %i has taken a fork\n", info->time_stamp / 1000, id);
		printf("%ld %i is eating.\n", info->time_stamp / 1000, id);
		philo->last_meal = info->time_stamp;
		usleep((info->time_to_eat * 1000)
			- (info->time_stamp - info->previous_stamp));
	}
	pthread_mutex_unlock(&info->forks[id]);
	pthread_mutex_unlock(&info->forks[left]);
	return (1);
}

void	go_to_sleep(int id, t_info	*info)
{
	info->previous_stamp = info->time_stamp;
	printf("%ld %i is sleeping\n", info->time_stamp / 1000, id);
	usleep((info->time_to_sleep * 1000) - (info->time_stamp - info->previous_stamp));
}

void	*philo_thread(void *vargp)
{
	t_philo	*philo;
	int				state;

	philo = (t_philo *)vargp;
	state = 1;
	while (state)
	{
		state = try_to_eat(philo->id, philo->info);
		if (state)
			go_to_sleep(philo->id, philo->info);
	}
	printf("%ld %i died\n", philo->info->time_stamp / 1000, philo->id);
	return (NULL);
}
