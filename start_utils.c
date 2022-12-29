/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   start_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltombell <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/29 12:31:46 by ltombell          #+#    #+#             */
/*   Updated: 2022/12/29 16:21:02 by ltombell         ###   ########.fr       */
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

void	ft_start_mutexes(t_program *prg)
{
	int	i;

	i = 0;
	prg->forks = malloc (sizeof(pthread_mutex_t) * prg->philo_nbr);
	while (i < prg->philo_nbr)
	{
		pthread_mutex_init(&prg->forks[i], NULL);
		i++;
	}
	pthread_mutex_init(&prg->write, NULL);
	pthread_mutex_init(&prg->eat_check, NULL);
}

void	ft_start_philos(t_program *prg)
{
	int	i;

	i = 0;
	prg->philos = malloc (sizeof(t_philo) * prg->philo_nbr);
	while (i < prg->philo_nbr)
	{
		prg->philos[i].id = i;
		prg->philos[i].left_fork = i;
		if (i < prg->philo_nbr - 1)
			prg->philos[i].right_fork = i + 1;
		else
			prg->philos[i].right_fork = 0;
		prg->philos[i].times_ate = 0;
		prg->philos[i].prg = prg;
		i++;
	}
}

int	ft_has_everyone_enough(t_program *prg)
{
	int	i;
	int	count;

	i = 0;
	count = 0;
	while (i < prg->philo_nbr)
	{
		if (prg->philos[i].times_ate >= prg->max_meals - 1)
			count++;
		i++;
	}
	if (count == prg->philo_nbr)
		return (1);
	else
		return (0);
}

void	lone_philo(t_program *prg)
{
	printf("0	1 has taken a fork\n");
	ft_usleep(prg->die_time);
	printf("%ld	1 died\n", get_current_time(prg->start_time));
}
