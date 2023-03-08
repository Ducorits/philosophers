/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   philo.c                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: dritsema <dritsema@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/01/11 20:07:04 by dritsema      #+#    #+#                 */
/*   Updated: 2023/03/08 19:34:25 by dritsema      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <philo.h>
#include <stdio.h>
#include <sys/time.h>
#include <unistd.h>

int	choose_fork_id(t_philo *philo, int *left, int *right)
{
	if (philo->id % 2)
	{
		*left = philo->id;
		*right = (philo->id + 1) % philo->info->philo_count;
	}
	else
	{
		if (philo->id)
			*left = philo->id - 1;
		else
			*left = philo->info->philo_count - 1;
		*right = philo->id;
	}
	return (0);
}

int	grab_forks(t_philo *philo, t_info *info, int left, int right)
{
	if (!pthread_mutex_lock(&info->forks[left]))
	{
		if (info->sim_end)
		{
			pthread_mutex_unlock(&info->forks[left]);
			return (0);
		}
		printf("%ld %i has taken a fork\n", info->time_stamp / 1000, philo->id);
		if (!pthread_mutex_lock(&info->forks[right]))
		{
			if (info->sim_end)
			{
				pthread_mutex_unlock(&info->forks[right]);
				pthread_mutex_unlock(&info->forks[left]);
				return (0);
			}
			printf("%ld %i has taken a fork\n",
				info->time_stamp / 1000, philo->id);
			return (1);
		}
	}
	return (1);
}

int	try_to_eat(t_philo *philo)
{
	int		left;
	int		right;
	t_info	*info;

	info = philo->info;
	choose_fork_id(philo, &left, &right);
	if (grab_forks(philo, info, left, right))
	{
		printf("%ld %i is eating.\n", info->time_stamp / 1000, philo->id);
		philo->times_eaten++;
		philo->last_meal = info->time_stamp;
		custom_sleep(info->time_to_eat * 1000);
		pthread_mutex_unlock(&info->forks[left]);
		pthread_mutex_unlock(&info->forks[right]);
		return (1);
	}
	return (0);
}

void	go_to_sleep(int id, t_info	*info)
{
	printf("%ld %i is sleeping\n", info->time_stamp / 1000, id);
	custom_sleep(info->time_to_sleep * 1000);
}

void	*philo_thread(void *vargp)
{
	t_philo	*philo;

	philo = (t_philo *)vargp;
	pthread_mutex_lock(&philo->info->start);
	pthread_mutex_unlock(&philo->info->start);
	while (philo->info->sim_end == 0)
	{
		if (try_to_eat(philo) && !philo->info->sim_end)
			go_to_sleep(philo->id, philo->info);
		printf("%ld %i is thinking.\n",
			philo->info->time_stamp / 1000, philo->id);
	}
	return (NULL);
}
