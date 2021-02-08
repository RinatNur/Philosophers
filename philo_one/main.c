/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jheat <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/31 18:56:55 by jheat             #+#    #+#             */
/*   Updated: 2021/01/31 18:57:00 by jheat            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int				check_life_time(t_phil *phil)
{
	long			time_now;

	time_now = (long)get_time();
	if (g_data.is_dead == 1)
		return (1);
	if (time_now - phil->last_eating > g_data.params.time_to_die
			&& phil->is_eating != 1)
	{
		pthread_mutex_lock(&g_data.print);
		g_data.is_dead = 1;
		print_action_dead(phil, DIE);
		return (1);
	}
	return (0);
}

static int		check_death_of_phil(t_phil *phil)
{
	int i;
	int flag;

	while (1)
	{
		if (g_data.is_dead == 1)
			return (0);
		i = 0;
		flag = 0;
		while (i < PHILS_N)
		{
			if (phil[i].remain_eating_times == 0)
				flag++;
			i++;
		}
		if (flag == PHILS_N)
			return (0);
		usleep(50);
	}
}

static int		parser(int argc, char **argv)
{
	if (6 < argc || argc < 5)
		return (1);
	if (atoi_mini(&PHILS_N, argv[1]) || 200 < PHILS_N || PHILS_N < 2)
		return (1);
	if (atoi_mini(&g_data.params.time_to_die, argv[2]))
		return (1);
	if (atoi_mini(&g_data.params.time_to_eat, argv[3]))
		return (1);
	if (atoi_mini(&g_data.params.time_to_sleep, argv[4]))
		return (1);
	if (argc == 6)
	{
		if (atoi_mini(&g_data.params.num_of_eating_times, argv[5]))
			return (1);
		if (g_data.params.num_of_eating_times == 0)
			return (1);
	}
	if (argc == 5)
		g_data.params.num_of_eating_times = -1;
	return (0);
}

static void		processing(void)
{
	pthread_mutex_t		fork_mutex[PHILS_N];
	t_phil				phil[PHILS_N];
	int					i;

	i = -1;
	g_data.fork_mutex = fork_mutex;
	while (++i < PHILS_N)
	{
		phil[i].index = i + 1;
		phil[i].last_eating = get_time();
		phil[i].remain_eating_times = g_data.params.num_of_eating_times;
		pthread_mutex_init(&g_data.fork_mutex[i], NULL);
	}
	pthread_mutex_init(&g_data.print, NULL);
	g_data.start_time = get_time();
	i = -1;
	while (++i < PHILS_N)
	{
		phil[i].left_fork = i + 1;
		phil[i].is_eating = 0;
		phil[i].right_fork = (i == 0) ? PHILS_N : i;
		pthread_create(&phil[i].thread, NULL, &feast_func, &phil[i]);
	}
	if (check_death_of_phil(phil) == 0)
		return ;
}

int				main(int argc, char **argv)
{
	g_data.is_dead = 0;
	g_data.fork_mutex = NULL;
	if (parser(argc, argv))
	{
		ft_write(2, "Arguments are not valid");
		return (1);
	}
	processing();
	return (0);
}
