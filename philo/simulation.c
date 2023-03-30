/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltombell <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/29 12:53:58 by ltombell          #+#    #+#             */
/*   Updated: 2023/01/04 17:56:53 by ltombell         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	ft_eat(t_philo *philo)
{
	pthread_mutex_lock(&philo->prg->forks[choose_fork_a
		(philo->left_fork, philo->right_fork)]);
	ft_unlock_print_lock(philo->prg, philo->id, "has taken a fork");
	pthread_mutex_lock(&philo->prg->forks[choose_fork_b
		(philo->right_fork, philo->left_fork)]);
	ft_unlock_print_lock(philo->prg, philo->id, "has taken a fork");
	ft_unlock_print_lock(philo->prg, philo->id, "is eating");
	pthread_mutex_lock(&philo->prg->eat_check);
	philo->last_eat = get_ms();
	philo->times_ate++;
	pthread_mutex_unlock(&philo->prg->eat_check);
	ft_usleep(philo->prg->eat_time);
	pthread_mutex_unlock(&philo->prg->forks[choose_fork_a
		(philo->left_fork, philo->right_fork)]);
	pthread_mutex_unlock(&philo->prg->forks[choose_fork_b
		(philo->right_fork, philo->left_fork)]);
}

void	*routine(void *philosopher)
{
	t_philo	*philo;

	philo = (t_philo *) philosopher;
	if (philo->id % 2 != 0)
		usleep(15000);
	while (!check_loop(philo))
	{
		ft_eat(philo);
		pthread_mutex_lock(&philo->prg->eat_check);
		if (philo->prg->eat_end == 1)
		{
			pthread_mutex_unlock(&philo->prg->eat_check);
			break ;
		}
		pthread_mutex_unlock(&philo->prg->eat_check);
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
	pthread_mutex_destroy(&prg->eat_check);
	free(prg->philos);
	free(prg->forks);
}

void	ft_black_death(t_program *prg)
{
	int		i;
	time_t	tmp;

	i = 0;
	while (prg->eat_end == 0)
	{
		pthread_mutex_lock(&prg->eat_check);
		tmp = get_current_time(prg->philos[i].last_eat);
		pthread_mutex_unlock(&prg->eat_check);
		while (i < prg->philo_nbr && prg->die_end == 0)
		{
			if (tmp > prg->die_time)
				ft_time_to_die(prg, i);
			usleep(100);
			i++;
		}
		if (prg->die_end == 1)
			break ;
		i = 0;
		pthread_mutex_lock(&prg->eat_check);
		if (ft_has_everyone_enough(prg) == 1 && prg->max_meals != -1)
			prg->eat_end = 1;
		pthread_mutex_unlock(&prg->eat_check);
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
		pthread_mutex_lock(&prg->eat_check);
		prg->philos[i].last_eat = prg->start_time;
		pthread_mutex_unlock(&prg->eat_check);
		i++;
	}
	ft_black_death(prg);
	ft_free_n_quit(prg);
	return ;
}
