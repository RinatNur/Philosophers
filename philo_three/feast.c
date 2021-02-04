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

static void		take_fork(t_phil *all)
{
	sem_wait(g_forks);
	print_action(all, FORK);
}

void			*feast_func(void *phil)
{
	t_phil		*all;


	all = (t_phil *)phil;
	while (1)
	{
		if (all->remain_eating_times == 0 || g_data.is_dead == 1)
			return (NULL);
		take_fork(all);
		take_fork(all);
		all->remain_eating_times--;
		do_when_phil_is_eating(all, get_time(), g_data.params.time_to_eat);
		sem_post(g_forks);
		sem_post(g_forks);
		print_action(all, SLEEP);
		check_sleeping_time(get_time(), g_data.params.time_to_sleep);
		print_action(all, THINK);
	}
}
