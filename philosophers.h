#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H
# include <pthread.h>
# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include <sys/time.h>

typedef struct s_program	t_program;

typedef struct	s_philosopher
{
	pthread_t thread;
	int	id;
	t_program	*prg;
	time_t	last_eat;
	int		times_eat;
}	t_philosopher;

typedef struct	s_program
{
	time_t	starttime;
	int	philosophers;
	int	die_time;
	int	eat_time;
	int	sleep_time;
	int	repetitions;
	pthread_mutex_t *forks;
	t_philosopher	**filosofi;
	int	i;
	int	end;
}	t_program;

#endif
