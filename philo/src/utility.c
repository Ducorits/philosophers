/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   utility.c                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: dritsema <dritsema@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/03/08 18:53:20 by dritsema      #+#    #+#                 */
/*   Updated: 2023/03/08 18:56:55 by dritsema      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <sys/time.h>
#include <unistd.h>

long	custom_sleep(long long sleep_time)
{
	struct timeval	start;
	struct timeval	current;
	long			to_sleep;
	long			start_usec;
	long			current_usec;

	gettimeofday(&start, NULL);
	current = start;
	to_sleep = sleep_time;
	start_usec = (start.tv_sec * 1000000) + start.tv_usec;
	current_usec = start_usec;
	while (current_usec - start_usec <= sleep_time)
	{
		if (to_sleep >= 2)
			to_sleep = sleep_time - (current_usec - start_usec) / 2;
		usleep(to_sleep);
		gettimeofday(&current, NULL);
		current_usec = (current.tv_sec * 1000000) + current.tv_usec;
	}
	return (current_usec - start_usec);
}
