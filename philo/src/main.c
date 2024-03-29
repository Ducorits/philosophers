/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   main.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: dritsema <dritsema@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/01/03 13:10:32 by dritsema      #+#    #+#                 */
/*   Updated: 2023/04/03 15:37:26 by dritsema      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <philo.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/time.h>

void	end_sim(t_info *info, pthread_t *id)
{
	free(id);
	clear_info(info, info->philo_count - 1);
}

void	start_sim(t_info *info)
{
	pthread_t		*id;
	t_philo			*philos;

	id = malloc(info->philo_count * sizeof(pthread_t));
	philos = malloc(info->philo_count * sizeof(t_philo));
	if (philos == NULL || id == NULL)
		return (free(philos), free(id));
	pthread_mutex_lock(&info->start);
	if (start_threads(info, id, philos))
	{
		pthread_mutex_unlock(&info->start);
		return (end_sim(info, id));
	}
	pthread_mutex_unlock(&info->start);
	join_threads(id, info->philo_count - 1);
	clear_philos(philos, info->philo_count - 1);
	end_sim(info, id);
	return ;
}

// arguments:
// philo count,
// time to die,
// time to eat,
// time to sleep,
// optional eat goal.
int	main(int argc, char **argv)
{
	t_info	*info;

	if (argc > 4 && argc < 7)
	{
		info = init(argc, argv);
		if (info == NULL)
		{
			printf("Invalid arguments or error :)\n");
			return (1);
		}
		start_sim(info);
	}
	else
	{
		printf("Usage: ./philosophers [philosopher count] ");
		printf("[time to die] [time to eat] [time to sleep]");
		printf(" {optional eat goal}\n");
	}
	return (0);
}
