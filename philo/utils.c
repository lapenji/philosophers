/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltombell <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/29 16:21:11 by ltombell          #+#    #+#             */
/*   Updated: 2023/01/02 13:55:42 by ltombell         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

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

void	ft_usleep(int howlong)
{
	time_t	tmp;

	tmp = get_ms() + howlong;
	while (get_ms() < tmp)
		usleep(100);
}

void	ft_unlock_print_lock(t_program *prg, int i, char *message)
{
	int	tmp;

	pthread_mutex_lock(&prg->eat_check);
	tmp = prg->die_end;
	pthread_mutex_unlock(&prg->eat_check);
	pthread_mutex_lock(&prg->write);
	if (tmp == 0)
		printf("%ld	%d %s\n", get_current_time(prg->start_time), i + 1, message);
	pthread_mutex_unlock(&prg->write);
}
