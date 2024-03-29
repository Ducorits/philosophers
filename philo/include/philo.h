/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   philo.h                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: dritsema <dritsema@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/01/03 13:10:26 by dritsema      #+#    #+#                 */
/*   Updated: 2023/04/03 15:00:26 by dritsema      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <pthread.h>
# include <sys/time.h>

typedef enum e_state
{
	DEAD,
	ALIVE,
}	t_state;
typedef struct s_info
{
	int				philo_count;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				time_to_think;
	int				eat_goal;
	pthread_mutex_t	*forks;
	pthread_mutex_t	start;
}	t_info;

typedef struct s_philo
{
	t_state			state;
	t_info			*info;
	pthread_mutex_t	*first_fork;
	pthread_mutex_t	*second_fork;
	pthread_mutex_t	eat_lock;
	pthread_mutex_t	time_lock;
	pthread_mutex_t	state_lock;
	int				id;
	long			start_time;
	long			last_meal;
	int				times_eaten;

}	t_philo;

t_info	*init(int argc, char **argv);
void	kill_them_all(t_philo *philos, t_info *info);
void	*philo_thread(void *vargp);
void	choose_fork_even(t_philo *philo, int i);
void	choose_fork_uneven(t_philo *philo, int i);
void	*monitor_thread(void *vargp);
long	get_timestamp(long start_time);
void	custom_sleep(long sleep_time);
long	ms_stamp(t_philo *philo);
void	deconstruct_philo(t_philo *philo);
int		start_threads(t_info *info, pthread_t *id, t_philo *philos);

void	join_threads(pthread_t *id, int count);
void	clear_info(t_info *info, int count);
void	clear_philos(t_philo *philos, int count);

#endif //PHILO_H
