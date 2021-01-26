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
	if (atoi_mini(&data->params.num_of_ph, argv[1]) || data->params.num_of_ph > 200)
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

ssize_t		ft_write(int fd, const void *buf)
{
	int		len;
	int		ret;

	len = ft_strlen(buf);
	((ret = write(fd, buf, len)) == -1)
	? print_error("Error in function write", 3) : 0;
	return (ret);
}


void 	print_action(t_phil *all, int phil, char *str, long int action_time)
{
	long int 	time;

	pthread_mutex_lock(&all->data->print);
	time = action_time - all->data->start_time;
	ft_putnbr_fd(time, 1);
	ft_write(1, " ");
	ft_putnbr_fd((all->left_fork), 1);
	ft_write(1, str);
	pthread_mutex_unlock(&all->data->print);
}

void 	take_left_fork(t_phil *all, int left)
{
	pthread_mutex_lock(&all->data->fork_mutex[all->left_fork]);
	all->action_time = get_time();
	print_action(all, left, FORK_L, all->action_time);
}

void 	take_right_fork(t_phil *all, int right)
{
	pthread_mutex_lock(&all->data->fork_mutex[all->right_fork]);
	all->action_time = get_time();
	print_action(all, right, FORK_R, all->action_time);
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
	all->action_time = get_time();
	print_action(all, left, EAT, all->action_time);
	all->last_eating = get_time();
	true_sleep(all->action_time, all->data->params.time_to_eat);
	pthread_mutex_unlock(&all->data->fork_mutex[all->left_fork]);
	pthread_mutex_unlock(&all->data->fork_mutex[all->right_fork]);
	all->action_time = get_time();
	print_action(all, left, SLEEP, all->action_time);
	true_sleep(all->action_time, all->data->params.time_to_sleep);
	all->action_time = get_time();
	print_action(all, left, THINK, all->action_time);
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
	long int	time_now;
	static int i = 0;

	time_now = get_time();
	if (time_now - phil->last_eating > phil->data->params.time_to_die)
	{
		print_action(phil, phil->index, DIE, time_now);
		return (1);
	}
	usleep(50);
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
		return ;
}

int 	main(int argc, char **argv)
{
	t_data		data;

	if (parser(&data, argc, argv))
		print_error("Arguments are not valid", 1);
	processing(&data);

	return (0);
}
