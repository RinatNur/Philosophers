/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   feast.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jheat <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/31 18:58:19 by jheat             #+#    #+#             */
/*   Updated: 2021/01/31 18:58:22 by jheat            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

static void		do_when_phil_is_eating(t_phil *all, long start,
										long time_to_sleep)
{
	print_action(all, EAT);
	all->last_eating = get_time();
	all->is_eating = 1;
	while ((get_time() - start) < time_to_sleep)
		usleep(100);
	all->is_eating = 0;
}

static void		check_sleeping_time(long start, long time_to_sleep)
{
	while ((get_time() - start) < time_to_sleep)
		usleep(100);
}

static void		take_fork(t_phil *phil)
{
	sem_wait(g_forks);
	print_action(phil, FORK);
}

int			feast_func(t_phil *phil)
{
	pthread_create(&phil->thread, NULL, &check_death_of_phil, phil);
	while (1)
	{
		if (phil->remain_eating_times == 0 || g_data.is_dead == 1)
			exit (2);
		take_fork(phil);
		take_fork(phil);
		phil->remain_eating_times--;
		do_when_phil_is_eating(phil, get_time(), g_data.params.time_to_eat);
		sem_post(g_forks);
		sem_post(g_forks);
		print_action(phil, SLEEP);
		check_sleeping_time(get_time(), g_data.params.time_to_sleep);
		print_action(phil, THINK);
	}
}
