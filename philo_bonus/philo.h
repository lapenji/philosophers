/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltombell <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/04 17:37:46 by ltombell          #+#    #+#             */
/*   Updated: 2023/01/04 18:00:07 by ltombell         ###   ########.fr       */
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
# include <semaphore.h>
# include <fcntl.h>
# include <sys/stat.h>
# include <sys/wait.h>

typedef struct s_program	t_program;

typedef struct s_philo
{
	int			id;
	int			times_ate;
	time_t		last_eat;
	pthread_t	thread;
	pid_t		pid;
	t_program	*prg;
}	t_philo;

typedef struct s_program
{
	int		philo_nbr;
	int		die_time;
	int		eat_time;
	int		sleep_time;
	int		max_meals;
	int		die_end;
	int		eat_end;
	time_t	start_time;
	sem_t	*eat_check;
	sem_t	*forks;
	sem_t	*write;
	sem_t	*forthread;
	t_philo	*philos;
}	t_program;

void	ft_unlock_print_lock(t_program *prg, int i, char *message);
time_t	get_ms(void);
time_t	get_current_time(time_t start);
int		ft_atoi(const char *nptr);
void	ft_usleep(int howlong);
void	ft_fill_prg(t_program *prg, int argc, char **argv);
void	ft_start_semaphores(t_program *prg);
void	ft_start_philos(t_program *prg);
void	ft_start_processes(t_program *prg);
void	lone_philo(t_program *prg);
void	ft_free_n_quit(t_program *prg);
void	ft_close_proc(t_program *prg);
void	ft_time_to_die(t_philo *philo);
int		check_loop(t_philo *philo);

#endif
