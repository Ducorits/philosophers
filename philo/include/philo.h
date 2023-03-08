/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   philo.h                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: dritsema <dritsema@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/01/03 13:10:26 by dritsema      #+#    #+#                 */
/*   Updated: 2023/03/08 18:54:18 by dritsema      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <pthread.h>
# include <sys/time.h>

typedef struct s_info
{
	int				sim_end;
	int				someone_died;
	int				philo_count;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				eat_goal;
	long			time_stamp;
	long			previous_stamp;
	pthread_mutex_t	*forks;
	pthread_mutex_t	start;
}	t_info;

typedef struct s_philo
{
	int				id;
	t_info			*info;
	long			last_meal;
	int				times_eaten;
}	t_philo;

t_info	init(int argc, char **argv);
void	*philo_thread(void *vargp);
void	*monitor_thread(void *vargp);
long	custom_sleep(long long sleep_time);

#endif //PHILO_H
