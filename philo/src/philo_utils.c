/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   philo_utils.c                                      :+:    :+:            */
/*                                                     +:+                    */
/*   By: dritsema <dritsema@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/03/16 15:12:30 by dritsema      #+#    #+#                 */
/*   Updated: 2023/04/03 15:35:54 by dritsema      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <philo.h>

long	get_timestamp(long start_time)
{
	struct timeval	time;

	gettimeofday(&time, NULL);
	return ((((time.tv_sec * 1000000) + time.tv_usec)
			- start_time));
}

void	kill_them_all(t_philo *philos, t_info *info)
{
	int	i;

	i = 0;
	while (i < info->philo_count)
	{
		pthread_mutex_lock(&philos[i].state_lock);
		philos[i].state = DEAD;
		pthread_mutex_unlock(&philos[i].state_lock);
		i++;
	}
}

void	choose_fork_uneven(t_philo *philo, int i)
{
	t_info	*info;

	info = philo->info;
	if (i == info->philo_count - 1)
	{
		philo->first_fork = &info->forks[0];
		philo->second_fork = &info->forks[i];
	}
	else
	{
		if (i % 2)
		{
			philo->first_fork = &info->forks[i + 1];
			philo->second_fork = &info->forks[i];
		}
		else
		{
			philo->first_fork = &info->forks[i];
			philo->second_fork = &info->forks[i + 1];
		}
	}
}

void	choose_fork_even(t_philo *philo, int i)
{
	t_info	*info;

	info = philo->info;
	if (i % 2)
	{
		philo->first_fork = &info->forks[i];
		philo->second_fork = &info->forks[(i + 1) % info->philo_count];
	}
	else
	{
		if (i)
			philo->first_fork = &info->forks[i - 1];
		else
			philo->first_fork = &info->forks[info->philo_count - 1];
		philo->second_fork = &info->forks[i];
	}
}
