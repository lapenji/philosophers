/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltombell <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/02 15:08:24 by ltombell          #+#    #+#             */
/*   Updated: 2023/01/04 18:01:19 by ltombell         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	ft_eat(t_philo *philo)
{
	sem_wait(philo->prg->forks);
	ft_unlock_print_lock(philo->prg, philo->id, "has taken a fork");
	sem_wait(philo->prg->forks);
	ft_unlock_print_lock(philo->prg, philo->id, "has taken a fork");
	ft_unlock_print_lock(philo->prg, philo->id, "is eating");
	sem_wait(philo->prg->eat_check);
	philo->last_eat = get_ms();
	philo->times_ate++;
	sem_post(philo->prg->eat_check);
	ft_usleep(philo->prg->eat_time);
	sem_post(philo->prg->forks);
	sem_post(philo->prg->forks);
}

void	*ft_black_death(void *philosopher)
{
	t_philo	*philo;

	philo = (t_philo *)philosopher;
	while (666)
	{
		sem_wait(philo->prg->eat_check);
		if (get_current_time(philo->last_eat) > philo->prg->die_time)
			ft_time_to_die(philo);
		sem_post(philo->prg->eat_check);
		if (philo->prg->die_end == 1)
			break ;
		sem_wait(philo->prg->eat_check);
		if (philo->times_ate >= philo->prg->max_meals
			&& philo->prg->max_meals != -1)
		{
			sem_post(philo->prg->eat_check);
			break ;
		}
		sem_post(philo->prg->eat_check);
	}
	return (NULL);
}

void	ft_real_routine(t_philo *philo)
{
	int	tmp;

	while (!check_loop(philo))
	{
		ft_eat(philo);
		sem_wait(philo->prg->eat_check);
		if (philo->times_ate >= philo->prg->max_meals
			&& philo->prg->max_meals != -1)
		{
			sem_post(philo->prg->eat_check);
			break ;
		}
		sem_post(philo->prg->eat_check);
		ft_unlock_print_lock(philo->prg, philo->id, "is sleeping");
		ft_usleep(philo->prg->sleep_time);
		ft_unlock_print_lock(philo->prg, philo->id, "is thinking");
		sem_wait(philo->prg->forthread);
		tmp = philo->prg->die_end;
		sem_post(philo->prg->forthread);
	}
}

void	ft_routine(void *philosopher)
{
	t_philo	*philo;

	philo = (t_philo *) philosopher;
	philo->last_eat = get_ms();
	pthread_create(&philo->thread, NULL, ft_black_death, philosopher);
	if (philo->id % 2 != 0)
		usleep(15000);
	ft_real_routine(philo);
	pthread_join(philo->thread, NULL);
	ft_free_n_quit(philo->prg);
	exit (1);
}

void	ft_start_processes(t_program *prg)
{
	int	i;

	i = 0;
	prg->start_time = get_ms();
	while (i < prg->philo_nbr)
	{
		prg->philos[i].pid = fork();
		if (prg->philos[i].pid == 0)
			ft_routine(&prg->philos[i]);
		usleep(100);
		i++;
	}
	ft_close_proc(prg);
	ft_free_n_quit(prg);
	return ;
}
