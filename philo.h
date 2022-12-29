/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltombell <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/29 16:21:24 by ltombell          #+#    #+#             */
/*   Updated: 2022/12/29 16:21:54 by ltombell         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <unistd.h>
# include <string.h>
# include <stdio.h>
# include <stdlib.h>
# include <pthread.h>
# include <sys/time.h>

typedef struct s_program	t_program;

typedef struct s_philo
{
	int			id;
	int			times_ate;
	int			left_fork;
	int			right_fork;
	time_t		last_eat;
	pthread_t	thread;
	t_program	*prg;
}	t_philo;

typedef struct s_program
{
	int				philo_nbr;
	int				die_time;
	int				eat_time;
	int				sleep_time;
	int				max_meals;
	int				die_end;
	int				eat_end;
	time_t			start_time;
	pthread_mutex_t	eat_check;
	pthread_mutex_t	*forks;
	pthread_mutex_t	write;
	t_philo			*philos;
}	t_program;

int		ft_atoi(const char *nptr);
void	ft_usleep(int howlong);
time_t	get_ms(void);
time_t	get_current_time(time_t start);
void	ft_start_philos(t_program *prg);
void	ft_unlock_print_lock(t_program *prg, int i, char *message);
int		ft_has_everyone_enough(t_program *prg);
void	ft_start_threads(t_program *prg);
void	ft_fill_prg(t_program *prg, int argc, char **argv);
void	ft_start_mutexes(t_program *prg);
void	lone_philo(t_program *prg);

#endif
