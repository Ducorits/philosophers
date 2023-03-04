/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   philo.h                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: dritsema <dritsema@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/01/03 13:10:26 by dritsema      #+#    #+#                 */
/*   Updated: 2023/03/04 14:35:48 by dritsema      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <pthread.h>
# include <sys/time.h>

typedef enum state {
	IDLE,
	THINK,
	EAT,
	SLEEP
}	t_state;

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
}	t_info;

typedef struct s_philo
{
	t_state			state;
	int				id;
	t_info			*info;
	long			last_meal;
	int				times_eaten;
}	t_philo;

t_info	init(int argc, char **argv);
void	*philo_thread(void *vargp);

#endif //PHILO_H
