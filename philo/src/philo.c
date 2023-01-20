/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   philo.c                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: dritsema <dritsema@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/01/11 20:07:04 by dritsema      #+#    #+#                 */
/*   Updated: 2023/01/20 18:11:42 by dritsema      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <philo.h>
#include <stdio.h>
#include <sys/time.h>
#include <unistd.h>

int	custom_sleep(long long sleep_time)
{
	struct timeval	start;
	struct timeval	current;
	long long		to_sleep;
	long long		start_usec;
	long long		current_usec;

	gettimeofday(&start, NULL);
	current = start;
	to_sleep = sleep_time;
	start_usec = (start.tv_sec * 1000000) + start.tv_usec;
	current_usec = start_usec;
	while (current_usec - start_usec <= sleep_time)
	{
		to_sleep = to_sleep / 2;
		usleep(to_sleep);
		gettimeofday(&current, NULL);
		current_usec = (current.tv_sec * 1000000) + current.tv_usec;
	}
	return (0);
}

int	try_to_eat(t_philo *philo)
{
	int		left;
	t_info	*info;

	info = philo->info;
	if (philo->id != 0)
		left = philo->id - 1;
	else
		left = info->philo_count - 1;
	if (!pthread_mutex_lock(&info->forks[left]) && !pthread_mutex_lock(&info->forks[philo->id]))
	{
		if (info->time_stamp - philo->last_meal
			> info->time_to_die * 1000)
		{
			pthread_mutex_unlock(&info->forks[philo->id]);
			pthread_mutex_unlock(&info->forks[left]);
			return (0);
		}
		info->previous_stamp = info->time_stamp;
		printf("%ld %i has taken a fork\n", info->time_stamp / 1000, philo->id);
		printf("%ld %i has taken a fork\n", info->time_stamp / 1000, philo->id);
		printf("%ld %i is eating.\n", info->time_stamp / 1000, philo->id);
		philo->last_meal = info->time_stamp;
		custom_sleep(info->time_to_eat * 1000);
	}
	pthread_mutex_unlock(&info->forks[philo->id]);
	pthread_mutex_unlock(&info->forks[left]);
	return (1);
}

void	go_to_sleep(int id, t_info	*info)
{
	info->previous_stamp = info->time_stamp;
	printf("%ld %i is sleeping\n", info->time_stamp / 1000, id);
	custom_sleep(info->time_to_sleep * 1000);
}

void	*philo_thread(void *vargp)
{
	t_philo	*philo;
	int				state;

	philo = (t_philo *)vargp;
	state = 1;
	while (state)
	{
		state = try_to_eat(philo);
		if (state)
			go_to_sleep(philo->id, philo->info);
	}
	printf("%ld %i died\n", philo->info->time_stamp / 1000, philo->id);
	return (NULL);
}
