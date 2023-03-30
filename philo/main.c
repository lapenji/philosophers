/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltombell <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/29 12:17:46 by ltombell          #+#    #+#             */
/*   Updated: 2023/01/02 14:43:51 by ltombell         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	ft_time_to_die(t_program *prg, int i)
{
	ft_unlock_print_lock(prg, i, "died");
	pthread_mutex_lock(&prg->eat_check);
	prg->die_end = 1;
	pthread_mutex_unlock(&prg->eat_check);
}

int	choose_fork_a(int left, int right)
{
	if (left < right)
		return (left);
	return (right);
}

int	choose_fork_b(int right, int left)
{
	if (right < left)
		return (left);
	return (right);
}

int	ft_check_args(t_program *prg, int argc)
{
	if (prg->philo_nbr < 1)
		return (1);
	if (prg->die_time < 0)
		return (1);
	if (prg->eat_time < 0)
		return (1);
	if (prg->sleep_time < 0)
		return (1);
	if (argc == 6 && prg->max_meals < 1)
		return (1);
	return (0);
}

int	main(int argc, char **argv)
{
	t_program	prg;

	if (argc > 6 || argc < 5)
	{
		write (2, "Error:\nincorrect number of arguments\n", 38);
		return (-1);
	}
	ft_fill_prg(&prg, argc, argv);
	if (ft_check_args(&prg, argc) == 1)
	{
		write (2, "Error:\nat least one wrong argument\n", 36);
		return (-1);
	}
	if (prg.philo_nbr > 1)
	{
		ft_start_mutexes(&prg);
		ft_start_philos(&prg);
		ft_start_threads(&prg);
	}
	else
		lone_philo(&prg);
}
