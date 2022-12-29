/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltombell <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/29 12:53:58 by ltombell          #+#    #+#             */
/*   Updated: 2022/12/29 16:20:45 by ltombell         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	ft_eat(t_philo *philo)
{
	pthread_mutex_lock(&philo->prg->forks[philo->left_fork]);
	ft_unlock_print_lock(philo->prg, philo->id, "has taken a fork");
	pthread_mutex_lock(&philo->prg->forks[philo->right_fork]);
	ft_unlock_print_lock(philo->prg, philo->id, "has taken a fork");
	pthread_mutex_lock(&philo->prg->eat_check);
	ft_unlock_print_lock(philo->prg, philo->id, "is eating");
	philo->last_eat = get_ms();
	pthread_mutex_unlock(&philo->prg->eat_check);
	ft_usleep(philo->prg->eat_time);
	philo->times_ate++;
	pthread_mutex_unlock(&philo->prg->forks[philo->left_fork]);
	pthread_mutex_unlock(&philo->prg->forks[philo->right_fork]);
}

void	*routine(void *philosopher)
{
	t_philo	*philo;

	philo = (t_philo *) philosopher;
	if (philo->id % 2 != 0)
		usleep(15000);
	while (philo->prg->die_end == 0)
	{
		ft_eat(philo);
		if (philo->prg->eat_end == 1)
			break ;
		ft_unlock_print_lock(philo->prg, philo->id, "is sleeping");
		ft_usleep(philo->prg->sleep_time);
		ft_unlock_print_lock(philo->prg, philo->id, "is thinking");
	}
	return (NULL);
}

void	ft_free_n_quit(t_program *prg)
{
	int	i;

	i = 0;
	while (i < prg->philo_nbr)
	{
		pthread_join(prg->philos[i].thread, NULL);
		i++;
	}
	i = 0;
	while (i < prg->philo_nbr)
	{
		pthread_mutex_destroy(&prg->forks[i]);
		i++;
	}
	pthread_mutex_destroy(&prg->write);
	free(prg->philos);
	free(prg->forks);
}

void	ft_black_death(t_program *prg)
{
	int	i;

	i = 0;
	while (prg->eat_end == 0)
	{
		while (i < prg->philo_nbr && prg->die_end == 0)
		{
			pthread_mutex_lock(&prg->eat_check);
			if (get_current_time(prg->philos[i].last_eat) > prg->die_time)
			{
				ft_unlock_print_lock(prg, i, "died");
				prg->die_end = 1;
			}
			pthread_mutex_unlock(&prg->eat_check);
			usleep(100);
			i++;
		}
		if (prg->die_end == 1)
			break ;
		i = 0;
		if (ft_has_everyone_enough(prg) == 1 && prg->max_meals != -1)
			prg->eat_end = 1;
	}
}

void	ft_start_threads(t_program *prg)
{
	int	i;

	i = 0;
	prg->start_time = get_ms();
	while (i < prg->philo_nbr)
	{
		pthread_create(&prg->philos[i].thread, NULL, routine, &prg->philos[i]);
		prg->philos[i].last_eat = prg->start_time;
		i++;
	}
	ft_black_death(prg);
	ft_free_n_quit(prg);
	return ;
}
