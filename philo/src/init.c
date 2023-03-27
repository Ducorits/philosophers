/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   init.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: dritsema <dritsema@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/01/11 21:45:29 by dritsema      #+#    #+#                 */
/*   Updated: 2023/03/23 16:20:23 by dritsema      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <philo.h>
#include <stdlib.h>

static int	ft_isint(const char *str)
{
	while (*str)
	{
		if (*str < '0' || *str > '9')
			return (0);
		str++;
	}
	return (1);
}

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
	if (!info->forks)
		return (0);
	while (i < info->philo_count)
	{
		if (pthread_mutex_init(&info->forks[i], NULL))
			return (free(info->forks), 0);
		i++;
	}
	return (1);
}

t_info	*init(int argc, char **argv)
{
	t_info	*info;

	info = malloc(sizeof(t_info));
	if (!info)
		return (NULL);
	if (!ft_isint(argv[1]) || !ft_isint(argv[2])
		|| !ft_isint(argv[3]) || !ft_isint(argv[4]))
		return (free(info), NULL);
	if (argc == 6 && !ft_isint(argv[5]))
		return (free(info), NULL);
	info->philo_count = ft_atoi(argv[1]);
	info->time_to_die = ft_atoi(argv[2]);
	info->time_to_eat = ft_atoi(argv[3]);
	info->time_to_sleep = ft_atoi(argv[4]);
	if (argc == 6)
		info->eat_goal = ft_atoi(argv[5]);
	else
		info->eat_goal = -1;
	info->time_to_think = (info->time_to_eat * 2) - info->time_to_sleep;
	if (!init_mutexes(info))
		return (free(info), NULL);
	return (info);
}
