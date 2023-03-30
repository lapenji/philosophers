/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   useless_file.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltombell <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/04 17:54:42 by ltombell          #+#    #+#             */
/*   Updated: 2023/01/04 17:55:42 by ltombell         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	check_loop(t_philo *philo)
{
	int		loop;

	pthread_mutex_lock(&philo->prg->eat_check);
	loop = philo->prg->die_end;
	pthread_mutex_unlock(&philo->prg->eat_check);
	return (loop);
}
