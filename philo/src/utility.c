/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   utility.c                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: dritsema <dritsema@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/03/08 18:53:20 by dritsema      #+#    #+#                 */
/*   Updated: 2023/03/16 19:15:29 by dritsema      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <sys/time.h>
#include <unistd.h>

void	custom_sleep(long sleep_time)
{
	struct timeval	current;
	long			current_usec;
	long			goal;

	gettimeofday(&current, NULL);
	current_usec = (current.tv_sec * 1000000) + current.tv_usec;
	goal = current_usec + sleep_time;
	while (current_usec <= goal)
	{
		usleep(((goal - current_usec) / 3) * 2);
		gettimeofday(&current, NULL);
		current_usec = (current.tv_sec * 1000000) + current.tv_usec;
	}
}
