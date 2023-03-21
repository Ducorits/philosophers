/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   philo.c                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: dritsema <dritsema@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/01/11 20:07:04 by dritsema      #+#    #+#                 */
/*   Updated: 2023/03/21 14:30:10 by dritsema      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <philo.h>
#include <stdio.h>
#include <sys/time.h>
#include <unistd.h>

int	grab_forks(t_philo *philo)
{
	if (pthread_mutex_lock(philo->first_fork))
		return (0);
	pthread_mutex_lock(&philo->state_lock);
	if (!philo->state)
		return (pthread_mutex_unlock(&philo->state_lock),
			pthread_mutex_unlock(philo->first_fork), 0);
	pthread_mutex_unlock(&philo->state_lock);
	printf("%ld %i has taken a fork\n", ms_stamp(philo), philo->id);
	if (pthread_mutex_lock(philo->second_fork))
		return (pthread_mutex_unlock(philo->first_fork), 0);
	pthread_mutex_lock(&philo->state_lock);
	if (!philo->state)
	{
		pthread_mutex_unlock(philo->second_fork);
		pthread_mutex_unlock(philo->first_fork);
		return (pthread_mutex_unlock(&philo->state_lock), 0);
	}
	pthread_mutex_unlock(&philo->state_lock);
	printf("%ld %i has taken a fork\n", ms_stamp(philo), philo->id);
	return (1);
}

int	try_to_eat(t_philo *philo)
{
	t_info	*info;

	info = philo->info;
	if (grab_forks(philo))
	{
		printf("%ld %i is eating\n", ms_stamp(philo), philo->id);
		pthread_mutex_lock(&philo->eat_lock);
		philo->times_eaten++;
		pthread_mutex_unlock(&philo->eat_lock);
		pthread_mutex_lock(&philo->time_lock);
		philo->last_meal = get_timestamp(philo->start_time);
		pthread_mutex_unlock(&philo->time_lock);
		custom_sleep(info->time_to_eat * 1000);
		pthread_mutex_unlock(philo->first_fork);
		pthread_mutex_unlock(philo->second_fork);
		return (1);
	}
	return (0);
}

void	go_to_sleep(t_philo *philo, t_info	*info)
{
	pthread_mutex_lock(&philo->state_lock);
	if (philo->state)
	{
		pthread_mutex_unlock(&philo->state_lock);
		printf("%ld %i is sleeping\n", ms_stamp(philo), philo->id);
		custom_sleep(info->time_to_sleep * 1000);
	}
	else
		pthread_mutex_unlock(&philo->state_lock);
}

void	go_think(t_philo *philo)
{
	if (!philo->info->time_to_think)
		return ;
	pthread_mutex_lock(&philo->state_lock);
	if (philo->state)
	{
		pthread_mutex_unlock(&philo->state_lock);
		printf("%ld %i is thinking\n", ms_stamp(philo), philo->id);
		if (philo->info->philo_count % 2)
			custom_sleep(philo->info->time_to_think * 1000);
	}
	else
		pthread_mutex_unlock(&philo->state_lock);
}

void	*philo_thread(void *vargp)
{
	t_philo	*philo;

	philo = (t_philo *)vargp;
	pthread_mutex_lock(&philo->info->start);
	pthread_mutex_unlock(&philo->info->start);
	if (philo->info->philo_count == 1)
	{
		printf("%ld %i has taken a fork\n", ms_stamp(philo), philo->id);
		custom_sleep(philo->info->time_to_die * 1000);
		return (NULL);
	}
	while (1)
	{
		pthread_mutex_lock(&philo->state_lock);
		if (!philo->state)
			break ;
		pthread_mutex_unlock(&philo->state_lock);
		if (try_to_eat(philo))
		{
			go_to_sleep(philo, philo->info);
			go_think(philo);
		}
	}
	pthread_mutex_unlock(&philo->state_lock);
	return (NULL);
}
