/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   useless_file.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltombell <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/04 17:54:42 by ltombell          #+#    #+#             */
/*   Updated: 2023/01/04 18:01:34 by ltombell         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	check_loop(t_philo *philo)
{
	int		loop;

	sem_wait(philo->prg->forthread);
	loop = philo->prg->die_end;
	sem_post(philo->prg->forthread);
	return (loop);
}
