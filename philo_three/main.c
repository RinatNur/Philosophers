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

void			*check_death_of_phil(void *array)
{
	t_phil		*phil;
	int			i;

	phil = (t_phil *)array;
	while (1)
	{
		i = 0;
		while (i < PHILS_N)
		{
			if (get_time() - phil->last_eating > g_data.params.time_to_die
				&& phil->is_eating != 1)
			{
				sem_wait(g_print);
				g_data.is_dead = 1;
				print_action_dead(phil, DIE);
				unlink_sem();
				exit(3);
			}
			i++;
		}
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

void			check_exit_status(t_phil *phil)
{
	int			status;
	int			i;

	status = 0;
	while (1)
	{
		waitpid(-1, &status, 0);
		status = WEXITSTATUS(status);
		if (status == 3)
		{
			i = 0;
			while (i < PHILS_N)
				kill(phil[i++].pid, SIGKILL);
			return ;
		}
		else if (status == 2)
			break ;
	}
	i = 0;
	while (i < PHILS_N)
		kill(phil[i++].pid, SIGKILL);
}

static void		processing(void)
{
	t_phil		phil[PHILS_N];
	int			i;

	i = 0;
	g_data.start_time = get_time();
	while (i < PHILS_N)
	{
		phil[i].index = i + 1;
		phil[i].last_eating = get_time();
		phil[i].remain_eating_times = g_data.params.num_of_eating_times;
		phil[i].is_eating = 0;
		phil[i].pid = fork();
		if (phil[i].pid == 0)
			feast_func(&phil[i]);
		else if (phil[i].pid < 0)
			return ;
		usleep(100);
		i++;
	}
	check_exit_status(phil);
}

int				main(int argc, char **argv)
{
	unlink_sem();
	g_data.is_dead = 0;
	if (parser(argc, argv))
		print_error("Arguments are not valid", 1);
	g_forks = sem_open("g_forks", O_CREAT, 0666, PHILS_N);
	g_print = sem_open("g_print", O_CREAT, 0666, 1);
	processing();
	unlink_sem();
	return (0);
}
