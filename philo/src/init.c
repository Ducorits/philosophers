/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   init.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: dritsema <dritsema@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/01/11 21:45:29 by dritsema      #+#    #+#                 */
/*   Updated: 2023/03/14 15:04:34 by dritsema      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <philo.h>
#include <stdlib.h>

static int	ft_atoi(const char *str)
{
	unsigned int	n;
	int				mins;

	n = 0;
	mins = 1;
	while ((*str >= 9 && *str <= 13) || *str == ' ')
		str++;
	if (*str == '-')
		mins = -1;
	if (*str == '-' || *str == '+')
		str++;
	while (*str >= '0' && *str <= '9')
	{
		n = n * 10 + (*str - '0');
		str++;
	}
	return ((int)(n * mins));
}

int	init_mutexes(t_info *info)
{
	int	i;

	i = 0;
	info->forks = malloc(info->philo_count * sizeof(pthread_mutex_t));
	while (i < info->philo_count)
	{
		if (pthread_mutex_init(&info->forks[i], NULL))
			return (0);
		i++;
	}
	return (1);
}

t_info	*init(int argc, char **argv)
{
	t_info	*info;

	info = malloc(sizeof(t_info));
	info->philo_count = ft_atoi(argv[1]);
	info->time_to_die = ft_atoi(argv[2]);
	info->time_to_eat = ft_atoi(argv[3]);
	info->time_to_sleep = ft_atoi(argv[4]);
	if (argc == 6)
		info->eat_goal = ft_atoi(argv[5]);
	else
		info->eat_goal = 0;
	if (!init_mutexes(info))
		return (NULL);
	info->time_stamp = 0;
	return (info);
}
