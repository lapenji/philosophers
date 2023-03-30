/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltombell <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/29 17:22:22 by ltombell          #+#    #+#             */
/*   Updated: 2023/01/04 17:40:00 by ltombell         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	ft_free_n_quit(t_program *prg)
{
	sem_close(prg->forks);
	sem_close(prg->write);
	sem_close(prg->eat_check);
	sem_close(prg->forthread);
	sem_unlink("/forks");
	sem_unlink("/write");
	sem_unlink("/eat_check");
	sem_unlink("/thread");
	free(prg->philos);
}

void	ft_time_to_die(t_philo *philo)
{
	ft_unlock_print_lock(philo->prg, philo->id, "died");
	sem_wait(philo->prg->forthread);
	philo->prg->die_end = 1;
	ft_free_n_quit(philo->prg);
	exit (1);
}

void	ft_close_proc(t_program *prg)
{
	int	i;
	int	ret;

	i = 0;
	while (i < prg->philo_nbr)
	{
		waitpid(-1, &ret, 0);
		if (ret != 0)
		{
			i = 0;
			while (i < prg->philo_nbr)
			{
				kill(prg->philos[i].pid, 9);
				i++;
			}
			break ;
		}
		i++;
	}
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
		ft_start_semaphores(&prg);
		ft_start_philos(&prg);
		ft_start_processes(&prg);
	}
	else
		lone_philo(&prg);
}
