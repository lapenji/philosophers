#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/time.h>
#include "philosophers.h"

time_t	get_ms(void)
{
	struct timeval	time;

	gettimeofday(&time, NULL);
	return ((time.tv_sec * 1000) + (time.tv_usec / 1000));
}

time_t	get_current_time(time_t start)
{
	return (get_ms() - start);
}

int	ft_atoi(const char *nptr)
{
	int	i;
	int	result;
	int	sign;

	i = 0;
	result = 0;
	sign = 1;
	while (nptr[i] == 32 || (nptr[i] >= 9 && nptr[i] <= 13))
		i++;
	if (nptr[i] == '+' || nptr[i] == '-')
	{
		if (nptr[i] == '-')
			sign *= -1;
		i++;
	}
	while (nptr[i] >= '0' && nptr[i] <= '9')
	{
		result = result * 10 + nptr[i] - '0';
		i++;
	}
	return (result * sign);
}

int	ft_checknumbers(t_program *program, int argc)
{
	if (program->philosophers <= 0)
		return (1);
	if (program->die_time <= 0)
		return (1);
	if (program->eat_time <= 0)
		return (1);
	if (program->sleep_time <= 0)
		return (1);
	if (argc == 6 && program->repetitions <= 0)
		return (1);
	return (0);
}

void	eat_routine_even(t_philosopher *philo)
{
	time_t	tmp;

	pthread_mutex_lock(&philo->prg->forks[philo->id]);
	printf("%ld	%d has taken a fork\n", get_current_time(philo->prg->starttime), philo->id + 1);
	if (philo->id == philo->prg->philosophers - 1 && philo->prg->philosophers > 1)
		pthread_mutex_lock(&philo->prg->forks[0]);
	else
		pthread_mutex_lock(&philo->prg->forks[philo->id + 1]);
	printf("%ld	%d has taken a fork\n", get_current_time(philo->prg->starttime), philo->id + 1);
	printf("%ld	%d is eating\n", get_current_time(philo->prg->starttime), philo->id + 1);
	philo->last_eat = get_ms();
	tmp = get_ms() + (philo->prg->eat_time);
	while (get_ms() < tmp)
			usleep(100);
	philo->times_eat ++;
	pthread_mutex_unlock(&philo->prg->forks[philo->id]);
	if (philo->id != philo->prg->philosophers - 1)
		pthread_mutex_unlock(&philo->prg->forks[philo->id + 1]);
	else
		pthread_mutex_unlock(&philo->prg->forks[0]);
}


void	eat_routine_odd(t_philosopher *philo)
{
	time_t	tmp;

	if (philo->id != philo->prg->philosophers - 1)
		pthread_mutex_lock(&philo->prg->forks[philo->id + 1]);
	else
		pthread_mutex_lock(&philo->prg->forks[0]);
	printf("%ld	%d has taken a fork\n", get_current_time(philo->prg->starttime), philo->id + 1);
	pthread_mutex_lock(&philo->prg->forks[philo->id]);
	printf("%ld	%d has taken a fork\n", get_current_time(philo->prg->starttime), philo->id + 1);
	printf("%ld	%d is eating\n", get_current_time(philo->prg->starttime), philo->id + 1);
	philo->last_eat = get_ms();
	tmp = get_ms() + (philo->prg->eat_time);
	while (get_ms() < tmp)
			usleep(10);
	philo->times_eat ++;
	if (philo->id != philo->prg->philosophers - 1)
		pthread_mutex_unlock(&philo->prg->forks[philo->id + 1]);
	else
		pthread_mutex_unlock(&philo->prg->forks[0]);
	pthread_mutex_unlock(&philo->prg->forks[philo->id]);
}

int		has_everyone_enough(t_program *prg)
{
	int	i;
	int	count;

	i = 0;
	count = 0;
	while (i < prg->philosophers)
	{
		if (prg->filosofi[i]->times_eat >= prg->repetitions)
			count++;
		i++;
	}
	if (count == prg->philosophers)
		return (1);
	else
		return (0);
}

void	*black_death(void *a)
{
	t_program	*prg;
	int	i;

	i = 0;
	prg = (t_program *) a;
	while (prg->end == 0)
	{
		i = 0;
		while (i < prg->philosophers)
		{
			if (prg->repetitions != 0 && has_everyone_enough(prg) == 1)
				exit(0);
			if ((get_ms() - prg->filosofi[i]->last_eat) > prg->die_time)
			{
				printf("%ld	%d died\n", get_current_time(prg->starttime), i + 1);
				exit (0);
			}
			i++;
		}
	}
	return (NULL);
}

void	*routine(void *a)
{
	t_philosopher *ciccio;
	ciccio = (t_philosopher *) a;
	int	i;
	time_t	tmp;
	i = -1;
	if (ciccio->prg->repetitions > 0)
		i++;
	while (i != 9999999)
	{
		if (ciccio->id % 2 == 0)
			eat_routine_even(ciccio);
		else
			eat_routine_odd(ciccio);
		printf ("%ld	%d is sleeping\n", (get_ms() - ciccio->prg->starttime), ciccio->id + 1);
		tmp = get_ms() + ciccio->prg->sleep_time;
		while (get_ms() < tmp)
			usleep(100);
		printf ("%ld	%d is thinking\n", (get_ms() - ciccio->prg->starttime), ciccio->id + 1);
	}
	return (NULL);
}

int		main(int argc, char **argv)
{
	int	i;
	t_program	*program;
	pthread_t	morte;

	if (argc >= 5 && argc <= 6)
	{
		program = malloc(sizeof(t_program));
		program->philosophers = ft_atoi(argv[1]);
		program->die_time = ft_atoi(argv[2]);
		program->eat_time = ft_atoi(argv[3]);
		program->sleep_time = ft_atoi(argv[4]);
		if (argc == 6)
			program->repetitions = ft_atoi(argv[5]);
		else
			program->repetitions = 0;
		program->end = 0;
		if (ft_checknumbers(program, argc) == 1)
		{
			write (2, "Error:\nWrong parameter\n", 23);
			exit (-1);
		}
		program->filosofi = malloc(sizeof(t_philosopher *) * program->philosophers);
		program->forks = malloc (sizeof (pthread_mutex_t) * program->philosophers);
		i = 0;
		program->starttime = get_ms();
		while (i < program->philosophers)
		{
			program->filosofi[i] = malloc(sizeof (t_philosopher));
			program->filosofi[i]->id = i;
			program->filosofi[i]->prg = program;
			program->filosofi[i]->last_eat = program->starttime;
			pthread_mutex_init(&program->forks[i], NULL);
			pthread_create(&program->filosofi[i]->thread, NULL, &routine, (void *)program->filosofi[i]);
			i ++;
		}
		// i = 1;
		// //usleep(1);
		// while (i < program->philosophers)
		// {
		// 	program->filosofi[i] = malloc(sizeof (t_philosopher));
		// 	program->filosofi[i]->id = i;
		// 	program->filosofi[i]->prg = program;
		// 	program->filosofi[i]->last_eat = program->starttime;
		// 	pthread_mutex_init(&program->forks[i], NULL);
		// 	pthread_create(&program->filosofi[i]->thread, NULL, &routine, (void *)program->filosofi[i]);
		// 	i += 2;
		// }
		pthread_create(&morte, NULL, &black_death, (void *)program);
		i = 0;
		while (i < program->philosophers)
		{
			pthread_mutex_destroy(&program->forks[program->i]);
			pthread_join(program->filosofi[i]->thread, NULL);
			i++;
		}
		pthread_join(morte, NULL);
	}
}
