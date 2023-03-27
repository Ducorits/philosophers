/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   cleanup.c                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: dritsema <dritsema@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/03/27 13:56:26 by dritsema      #+#    #+#                 */
/*   Updated: 2023/03/27 15:08:08 by dritsema      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <philo.h>
#include <stdlib.h>

void	clear_ids(pthread_t *id, int count)
{
	while (count > -1)
	{
		pthread_join(id[count], NULL);
		count--;
	}
	free(id);
}

void	clear_info(t_info *info, int count)
{
	while (count > -1)
	{
		pthread_mutex_destroy(&info->forks[count]);
		count--;
	}
	pthread_mutex_destroy(&info->start);
	free(info->forks);
	free(info);
}

void	clear_philos(t_philo *philos, int count)
{
	while (count > -1)
	{
		pthread_mutex_destroy(&philos[count].state_lock);
		pthread_mutex_destroy(&philos[count].eat_lock);
		pthread_mutex_destroy(&philos[count].time_lock);
		count--;
	}
	free(philos);
}
