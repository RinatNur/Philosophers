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

static void		take_left_fork(t_phil *all, int left)
{
	(g_data.is_dead == 0)
	? pthread_mutex_lock(&g_data.fork_mutex[left]) : 0;
	print_action(all, FORK);
}

static void		take_right_fork(t_phil *all, int right)
{
	(g_data.is_dead == 0)
	? pthread_mutex_lock(&g_data.fork_mutex[right]) : 0;
	print_action(all, FORK);
}

void			*feast_func(void *phil)
{
	t_phil		*all;

	all = (t_phil *)phil;
	while (1)
	{
		if (all->remain_eating_times == 0)
			return (NULL);
		if (all->index % 2 != 0)
		{
			take_left_fork(all, all->left_fork);
			take_right_fork(all, all->right_fork);
		}
		else
		{
			take_right_fork(all, all->right_fork);
			take_left_fork(all, all->left_fork);
		}
		(all->remain_eating_times > 0) ? all->remain_eating_times-- : 0;
		do_when_phil_is_eating(all, get_time(), g_data.params.time_to_eat);
		(g_data.is_dead == 0) ? UNLOCK(&g_data.fork_mutex[all->left_fork]) : 0;
		(g_data.is_dead == 0) ? UNLOCK(&g_data.fork_mutex[all->right_fork]) : 0;
		print_action(all, SLEEP);
		check_sleeping_time(get_time(), g_data.params.time_to_sleep);
		print_action(all, THINK);
	}
}
