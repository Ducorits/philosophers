/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   monitor.c                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: dritsema <dritsema@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/03/08 16:39:53 by dritsema      #+#    #+#                 */
/*   Updated: 2023/04/03 15:36:28 by dritsema      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <philo.h>
#include <stdio.h>

int	check_death(t_info *info, t_philo *philo)
{
	long	time_stamp;

	time_stamp = get_timestamp(philo->start_time);
	pthread_mutex_lock(&philo->time_lock);
	if (time_stamp - philo->last_meal > info->time_to_die * 1000)
	{
		pthread_mutex_unlock(&philo->time_lock);
		printf("%ld %i died\n", time_stamp / 1000, philo->id);
		pthread_mutex_lock(&philo->state_lock);
		philo->state = DEAD;
		pthread_mutex_unlock(&philo->state_lock);
		return (1);
	}
	pthread_mutex_unlock(&philo->time_lock);
	return (0);
}

int	check_eatgoal(t_info *info, t_philo *philo)
{
	pthread_mutex_lock(&philo->eat_lock);
	if (philo->times_eaten < info->eat_goal || info->eat_goal == -1)
	{
		pthread_mutex_unlock(&philo->eat_lock);
		return (0);
	}
	pthread_mutex_unlock(&philo->eat_lock);
	return (1);
}

void	*monitor_thread(void *vargp)
{
	t_info	*info;
	t_philo	*philos;
	int		sim_end;
	int		i;
	int		eat_goal;

	sim_end = 0;
	philos = (t_philo *)vargp;
	info = philos[0].info;
	while (sim_end == 0)
	{
		i = 0;
		eat_goal = 0;
		while (i < info->philo_count && !sim_end)
		{
			sim_end = check_death(info, &philos[i]);
			eat_goal += check_eatgoal(info, &philos[i]);
			i++;
		}
		if (eat_goal >= info->philo_count)
			sim_end = 1;
	}
	kill_them_all(philos, info);
	return (NULL);
}
