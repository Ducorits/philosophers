/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   monitor.c                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: dritsema <dritsema@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/03/08 16:39:53 by dritsema      #+#    #+#                 */
/*   Updated: 2023/03/13 18:17:29 by dritsema      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <philo.h>
#include <stdio.h>

void	check_death(t_info *info, t_philo philo)
{
	if (info->time_stamp - philo.last_meal > info->time_to_die * 1000)
	{
		printf("%ld %i died\n", info->time_stamp / 1000, philo.id);
		info->someone_died = 1;
		info->sim_end = 1;
	}
	return ;
}

void	*monitor_thread(void *vargp)
{
	t_info	*info;
	t_philo	*philos;
	int		sim_end;
	int		i;
	int		eat_goal_reached;

	philos = (t_philo *)vargp;
	info = philos[0].info;
	while (sim_end == 0)
	{
		i = 0;
		eat_goal_reached = 1;
		while (i < info->philo_count && !sim_end)
		{
			pthread_mutex_lock(&philos[i].lock);
			check_death(info, philos[i]);
			if (philos[i].times_eaten < info->eat_goal || info->eat_goal == 0)
				eat_goal_reached = 0;
			pthread_mutex_unlock(&philos[i].lock);
			i++;
		}
		if (eat_goal_reached == 1)
		{
			pthread_mutex_lock(&info->lock);
			sim_end = 1;
			pthread_mutex_unlock(&info->lock);
		}
	}
	return (NULL);
}
