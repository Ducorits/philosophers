/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   philo.c                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: dritsema <dritsema@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/01/11 20:07:04 by dritsema      #+#    #+#                 */
/*   Updated: 2023/03/08 15:30:14 by dritsema      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <philo.h>
#include <stdio.h>
#include <sys/time.h>
#include <unistd.h>

long	custom_sleep(long long sleep_time)
{
	struct timeval	start;
	struct timeval	current;
	long			to_sleep;
	long			start_usec;
	long			current_usec;

	gettimeofday(&start, NULL);
	current = start;
	to_sleep = sleep_time;
	start_usec = (start.tv_sec * 1000000) + start.tv_usec;
	current_usec = start_usec;
	while (current_usec - start_usec <= sleep_time)
	{
		to_sleep = sleep_time - (current_usec - start_usec) / 2;
		usleep(to_sleep);
		gettimeofday(&current, NULL);
		current_usec = (current.tv_sec * 1000000) + current.tv_usec;
	}
	return (current_usec - start_usec);
}

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

int	try_to_eat(t_philo *philo)
{
	int		left;
	int		right;
	t_info	*info;

	info = philo->info;
	choose_fork_id(philo, &left, &right);
	if (!pthread_mutex_lock(&info->forks[left]))
	{
		if (info->someone_died)
		{
			pthread_mutex_unlock(&info->forks[left]);
			return (0);
		}
		printf("%ld %i has taken a fork\n", info->time_stamp / 1000, philo->id);
		pthread_mutex_lock(&info->forks[right]);
		if (info->someone_died)
		{
			pthread_mutex_unlock(&info->forks[right]);
			pthread_mutex_unlock(&info->forks[left]);
			return (0);
		}
		printf("%ld %i has taken a fork\n", info->time_stamp / 1000, philo->id);
		printf("%ld %i is eating.\n", info->time_stamp / 1000, philo->id);
		philo->times_eaten++;
		philo->last_meal = info->time_stamp;
		custom_sleep(info->time_to_eat * 1000);
		pthread_mutex_unlock(&info->forks[left]);
		pthread_mutex_unlock(&info->forks[right]);
		return (1);
	}
	// printf("pthread_mutex_lock failed\n");
	return (1);
}

void	go_to_sleep(int id, t_info	*info)
{
	// info->previous_stamp = info->time_stamp;
	printf("%ld %i is sleeping\n", info->time_stamp / 1000, id);
	custom_sleep(info->time_to_sleep * 1000);
}

void	*philo_thread(void *vargp)
{
	t_philo	*philo;
	int		state;

	philo = (t_philo *)vargp;
	state = 1;
	while (state && philo->info->someone_died == 0)
	{
		state = try_to_eat(philo);
		if (state)
			go_to_sleep(philo->id, philo->info);
	}
	if (philo->info->someone_died == 1)
		return (NULL);
	printf("%ld %i died\n", philo->info->time_stamp / 1000, philo->id);
	philo->info->someone_died = 1;
	return (NULL);
}
