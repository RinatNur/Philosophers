#include "philosophers.h"

static int		check_life_time(t_phil *phil)
{
	static int		i;
	long			time_now;

	i = 0;
	time_now = (long)get_time();
	if (time_now - phil->last_eating > phil->data->params.time_to_die
		&& phil->is_eating != 1)
	{
		pthread_mutex_lock(&phil->data->print);
		phil->data->is_dead = 1;
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
		i = 0;
		flag = 0;
		while (i < phil->PHILS_N)
		{
			if (check_life_time(&phil[i]))
				return (1);
			if (phil[i].remain_eating_times == 0)
				flag++;
			i++;
		}
		if (flag == phil->PHILS_N)
			return (1);
	}
}

static int		parser(t_data *data, int argc, char **argv)
{
	if (6 < argc || argc < 5 )
		return (1);
	if (atoi_mini(&PHILS_N, argv[1]) || 200 < PHILS_N || PHILS_N < 2)
		return (1);
	if (atoi_mini(&data->params.time_to_die, argv[2]))
		return (1);
	if (atoi_mini(&data->params.time_to_eat, argv[3]))
		return (1);
	if (atoi_mini(&data->params.time_to_sleep, argv[4]))
		return (1);
	if (argc == 6)
	{
		if (atoi_mini(&data->params.num_of_eating_times, argv[5]))
			return (1);
		if (data->params.num_of_eating_times == 0)
			return (1);
	}
	if (argc == 5)
		data->params.num_of_eating_times = -1;
	return (0);
}

static void		processing(t_data *data)
{
	t_phil		phil[PHILS_N];
	int			i;

	i = -1;
	if (!(data->fork_mutex = (t_mutex *)malloc(sizeof (t_mutex) * PHILS_N)))
		print_error("Memory not allocated", 3);
	while (++i < PHILS_N)
	{
		phil[i].data = data;
		phil[i].index = i + 1;
		phil[i].last_eating = get_time();
		phil[i].remain_eating_times = data->params.num_of_eating_times;
		pthread_mutex_init(&phil[i].data->fork_mutex[i] , NULL);
	}
	pthread_mutex_init(&data->print, NULL);
	data->start_time = get_time();
	i = -1;
	while (++i < PHILS_N)
	{
		phil[i].left_fork = i + 1;
		phil[i].right_fork = (i == 0) ? phil[i].PHILS_N : i;
		pthread_create(&phil[i].thread, NULL, &feast_func, &phil[i]);
	}
	if (check_death_of_phil(phil))
		return;
}

int				main(int argc, char **argv)
{
	t_data		data;
	int 		i;

	i = 0;
	data.is_dead = 0;
	if (parser(&data, argc, argv))
		print_error("Arguments are not valid", 1);
	processing(&data);
	usleep(1000);
	while (i < data.params.num_of_ph)
	{
		pthread_mutex_destroy(data.fork_mutex);
		i++;
	}
	return (0);
}
