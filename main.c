#include "philosophers.h"

void	true_sleep(long start, long time_to_sleep)
{
	while ((get_time() - start) < time_to_sleep)
		usleep(100);
}

int 	parser(t_data *data, int argc, char **argv)
{
	if (6 < argc || argc < 5 )
		return (1);
	if (atoi_mini(&data->params.num_of_ph, argv[1]) || 200 < data->params.num_of_ph || data->params.num_of_ph < 2)
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


void 	take_left_fork(t_phil *all, int left)
{
	pthread_mutex_lock(&all->data->fork_mutex[left]);
	print_action(all, FORK);
}

void 	take_right_fork(t_phil *all, int right)
{
	pthread_mutex_lock(&all->data->fork_mutex[right]);
	print_action(all, FORK);
}

void 	loop(t_phil *all, int left, int right)
{
	if (all->index % 2 != 0) {
		take_left_fork(all, left);
		take_right_fork(all, right);
	}
	else
	{
		take_right_fork(all, right);
		take_left_fork(all, left);
	}
	if (all->remain_eating_times > 0)
		all->remain_eating_times--;
	print_action(all, EAT);
	all->last_eating = get_time();
	all->is_eating = 1;
	true_sleep(get_time(), all->data->params.time_to_eat);
	all->is_eating = 0;
	pthread_mutex_unlock(&all->data->fork_mutex[left]);
	pthread_mutex_unlock(&all->data->fork_mutex[right]);
	print_action(all, SLEEP);
	true_sleep(get_time(), all->data->params.time_to_sleep);
	print_action(all, THINK);
}

void 	*func(void *phil)
{
	t_phil		*all;
	int 		left;
	int 		right;

	all = (t_phil *)phil;
	left = all->left_fork;
	right = all->right_fork;
	while (1)
	{
		loop(all, left, right);
	}
}

int 	check_life_time(t_phil *phil)
{
	long		time_now;
	static int i = 0;

	time_now = (long)get_time();
	if (time_now - phil->last_eating > phil->data->params.time_to_die && phil->is_eating != 1)
	{
		pthread_mutex_lock(&phil->data->print);
		phil->data->is_dead = 1;
		print_action_dead(phil, DIE);
		return (1);
	}
	return (0);
}

int 	check_death_of_phil(t_phil *phil)
{
	int i;
	int flag = 0;

	while (1)
	{
		i = 0;
		flag = 0;
		while (i < phil->data->params.num_of_ph)
		{
			if (check_life_time(&phil[i]))
				return (1);
			if (phil[i].remain_eating_times == 0)
				flag++;
			i++;
		}
		if (flag == phil->data->params.num_of_ph)
			return (1);
	}
}

void 	processing(t_data *data)
{
	t_phil		phil[data->params.num_of_ph];
	int 			i;
	int 			num_of_ph;

	i = 0;
	num_of_ph = data->params.num_of_ph;
	if (!(data->fork_mutex = (t_mutex *)malloc(sizeof (t_mutex) * data->params.num_of_ph)))
		print_error("Memory not allocated", 3);
	while (i < num_of_ph)
	{
		phil[i].data = data;
		phil[i].index = i + 1;
		phil[i].last_eating = get_time();
		phil[i].remain_eating_times = data->params.num_of_eating_times;
		pthread_mutex_init(&phil[i].data->fork_mutex[i] , NULL);
		i++;
	}
	pthread_mutex_init(&data->print, NULL);
	data->start_time = get_time();

	i = 0;
	while (i < num_of_ph)
	{

		phil[i].left_fork = i + 1;
		if (i == 0)
			phil[i].right_fork = phil[i].data->params.num_of_ph;
		else
			phil[i].right_fork = i;
		pthread_create(&phil[i].thread, NULL, &func, &phil[i]);
		i++;
	}
	if (check_death_of_phil(phil))
		return;
}


int 	main(int argc, char **argv)
{
	t_data		data;
	int 		i = 0;

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
