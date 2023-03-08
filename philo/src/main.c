/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   main.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: dritsema <dritsema@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/01/03 13:10:32 by dritsema      #+#    #+#                 */
/*   Updated: 2023/03/08 15:39:28 by dritsema      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <philo.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/time.h>

// long	get_time(void)
// {
// 	struct timeval	time;
// 	long			utime;

// 	gettimeofday(&time, NULL);
// 	utime = (time.tv_sec * 1000000) + time.tv_usec;
// 	return (utime);
// }

void	*monitor_thread(void *vargp)
{
	t_info	*info;
	t_philo	*philos;
	int		i;
	int		eat_goal_reached;

	philos = (t_philo *)vargp;
	info = philos[0].info;
	printf("%i\n", info->sim_end);
	while (info->sim_end == 0)
	{
		printf("what\n");
		i = 0;
		eat_goal_reached = 1;
		while (i < info->philo_count)
		{
			if (info->time_stamp - philos[i].last_meal
				> info->time_to_die * 1000)
			{
				printf("%ld %i died\n", info->time_stamp / 1000, philos[i].id);
				info->someone_died = 1;
				info->sim_end = 1;
			}
			if (philos[i].times_eaten < info->eat_goal)
				eat_goal_reached = 0;
			i++;
		}
		if (eat_goal_reached == 1)
			info->sim_end = 1;
	}
	return (NULL);
}

void	*time_thread(void *vargp)
{
	t_info			*info;
	struct timeval	stamp;
	struct timeval	start;

	gettimeofday(&start, NULL);
	info = (t_info *)vargp;
	while (1)
	{
		gettimeofday(&stamp, NULL);
		info->time_stamp = (((stamp.tv_sec * 1000000) + stamp.tv_usec)
				- ((start.tv_sec * 1000000) + start.tv_usec));
		usleep(1);
	}
	return (NULL);
}

void	end_sim(t_info *info)
{
	int	i;

	i = 0;
	while (i < info->philo_count)
	{
		pthread_mutex_destroy(&info->forks[i]);
		i++;
	}
	free(info->forks);
}

void	start_sim(t_info info)
{
	struct timeval	start;
	int				i;
	pthread_t		*id;
	t_philo			*philos;
	pthread_t		time_id;
	pthread_t		monit_id;

	i = 0;
	gettimeofday(&start, NULL);
	id = malloc(info.philo_count * sizeof(pthread_t));
	philos = malloc(info.philo_count * sizeof(t_philo));
	pthread_create(&time_id, NULL, time_thread, (void *)&info);
	pthread_detach(time_id);
	while (i < info.philo_count)
	{
		philos[i].id = i;
		philos[i].info = &info;
		philos[i].state = IDLE;
		philos[i].last_meal = ((start.tv_sec * 1000000) + start.tv_usec) / 1000;
		philos[i].times_eaten = 0;
		pthread_create(&id[i], NULL, philo_thread, (void *)&philos[i]);
		i++;
	}
	pthread_create(&monit_id, NULL, monitor_thread, (void *)&philos);
	pthread_detach(monit_id);
	i = 0;
	while (i < info.philo_count)
	{
		pthread_join(id[i], NULL);
		i++;
	}
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
	t_info	info;

	if (argc > 4 && argc < 7)
	{
		printf("init\n");
		info = init(argc, argv);
		printf("start_sim\n");
		start_sim(info);
		printf("end_sim\n");
		end_sim(&info);
	}
	else
	{
		write(STDOUT_FILENO, "Usage: ./philosophers [philosopher count] ", 43);
		write(STDOUT_FILENO, "[time to die] [time to eat] [time to sleep]", 44);
		write(STDOUT_FILENO, " {optional eat goal}\n", 22);
	}
	return (0);
}
