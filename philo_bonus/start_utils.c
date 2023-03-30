/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   start_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltombell <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/02 14:52:38 by ltombell          #+#    #+#             */
/*   Updated: 2023/01/03 17:34:35 by ltombell         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	ft_fill_prg(t_program *prg, int argc, char **argv)
{
	prg->philo_nbr = ft_atoi(argv[1]);
	prg->die_time = ft_atoi(argv[2]);
	prg->eat_time = ft_atoi(argv[3]);
	prg->sleep_time = ft_atoi(argv[4]);
	if (argc == 6)
		prg->max_meals = ft_atoi(argv[5]);
	else
		prg->max_meals = -1;
	prg->eat_end = 0;
	prg->die_end = 0;
	prg->start_time = get_ms();
}

void	ft_start_semaphores(t_program *prg)
{
	sem_unlink("/forks");
	sem_unlink("/write");
	sem_unlink("/eat_check");
	sem_unlink("/thread");
	prg->forks = sem_open("/forks", O_CREAT, S_IRWXU, prg->philo_nbr);
	prg->write = sem_open("/write", O_CREAT, S_IRWXU, 1);
	prg->eat_check = sem_open("/eat_check", O_CREAT, S_IRWXU, 1);
	prg->forthread = sem_open("/thread", O_CREAT, S_IRWXU, 1);
}

void	ft_start_philos(t_program *prg)
{
	int	i;

	i = 0;
	prg->philos = malloc (sizeof(t_philo) * prg->philo_nbr);
	while (i < prg->philo_nbr)
	{
		prg->philos[i].id = i;
		prg->philos[i].times_ate = 0;
		prg->philos[i].prg = prg;
		i++;
	}
}

void	lone_philo(t_program *prg)
{
	printf("0	1 has taken a fork\n");
	ft_usleep(prg->die_time);
	printf("%ld	1 died\n", get_current_time(prg->start_time));
}
