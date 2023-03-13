/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   philo.h                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: dritsema <dritsema@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/01/03 13:10:26 by dritsema      #+#    #+#                 */
/*   Updated: 2023/03/13 18:13:45 by dritsema      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <pthread.h>
# include <sys/time.h>

typedef enum e_state
{
	SIM_END,
	THINKING,
	EATING,
	SLEEPING,
	DEAD,
}	t_state;
typedef struct s_info
{
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
	t_state			state;
	int				id;
	t_info			*info;
	long			last_meal;
	int				times_eaten;
	pthread_mutex_t	lock;
}	t_philo;

t_info	*init(int argc, char **argv);
void	*philo_thread(void *vargp);
void	*monitor_thread(void *vargp);
long	custom_sleep(long long sleep_time);

#endif //PHILO_H
