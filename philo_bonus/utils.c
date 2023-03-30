/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltombell <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/02 15:23:39 by ltombell          #+#    #+#             */
/*   Updated: 2023/01/04 16:28:25 by ltombell         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	ft_unlock_print_lock(t_program *prg, int i, char *message)
{
	int	tmp;

	sem_wait(prg->forthread);
	tmp = prg->die_end;
	sem_post(prg->forthread);
	sem_wait(prg->write);
	if (tmp == 0)
		printf("%ld	%d %s\n", get_current_time(prg->start_time), i + 1, message);
	sem_post(prg->write);
}

time_t	get_ms(void)
{
	struct timeval	time;

	gettimeofday(&time, NULL);
	return ((time.tv_sec * 1000) + (time.tv_usec / 1000));
}

time_t	get_current_time(time_t start)
{
	return (get_ms() - start);
}

int	ft_atoi(const char *nptr)
{
	int	i;
	int	result;
	int	sign;

	i = 0;
	result = 0;
	sign = 1;
	while (nptr[i] == 32 || (nptr[i] >= 9 && nptr[i] <= 13))
		i++;
	if (nptr[i] == '+' || nptr[i] == '-')
	{
		if (nptr[i] == '-')
			sign *= -1;
		i++;
	}
	while (nptr[i] >= '0' && nptr[i] <= '9')
	{
		result = result * 10 + nptr[i] - '0';
		i++;
	}
	return (result * sign);
}

void	ft_usleep(int howlong)
{
	time_t	tmp;

	tmp = get_ms() + howlong;
	while (get_ms() < tmp)
		usleep(100);
}
