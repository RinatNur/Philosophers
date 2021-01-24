#include "philosophers.h"

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


void 	print_action(t_data *data, int phil, char *str, long int action_time)
{
	long int 	time;

	pthread_mutex_lock(&data->print);
	time = action_time - data->start_time;
	ft_putnbr_fd(time, 1);
	ft_write(1, " ");
	ft_putnbr_fd((data->left_fork), 1);
	ft_write(1, str);
	pthread_mutex_unlock(&data->print);
}

void 	loop(t_data *all, int left, int right)
{
	if (left % 2 == 0)
		usleep(100);
	pthread_mutex_lock(&all->phil[left].mutex);
	all->phil[left].action_time = get_time();
	print_action(all, left, FORK, all->phil[left].action_time);
	pthread_mutex_lock(&all->phil[right].mutex);
	all->phil[left].action_time = get_time();
	print_action(all, left, FORK, all->phil[left].action_time);
	if (all->params.num_of_eating_times > 0)
		all->params.num_of_eating_times--;
	all->phil[left].action_time = get_time();
	print_action(all, left, EAT, all->phil[left].action_time);
	usleep(all->params.time_to_eat * 1000);
	pthread_mutex_unlock(&all->phil[left].mutex);
	pthread_mutex_unlock(&all->phil[right].mutex);
	all->phil[left].action_time = get_time();
	print_action(all, left, SLEEP, all->phil[left].action_time);
	usleep(all->params.time_to_sleep * 1000);
	all->phil[left].action_time = get_time();
	print_action(all, left, THINK, all->phil[left].action_time);
}

void 	*func(void *data)
{
	t_data		*all;
	int 		left;
	int 		right;
	int 		num_of_phil;
	all = (t_data *)data;
	left = all->left_fork;
	right = all->right_fork;

	while (1)
	{
		loop(all, left, right);
		usleep(50);
	}
}

void 	processing(t_data *data)
{
	int 			i;

	i = 0;
	if (!(data->phil = (t_phil *)malloc(sizeof (t_phil) * data->params.num_of_ph)))
		print_error("Memory not allocated", 3);
	while (i < data->params.num_of_ph)
	{
		data->phil[i].index = i + 1;
		data->phil[i].last_eating = get_time();
		pthread_mutex_init(&data->phil[i].mutex , NULL);
		usleep(50);
		i++;
	}
	pthread_mutex_init(&data->print, NULL);
	data->start_time = get_time();

	i = 0;
	while (i < data->params.num_of_ph)
	{
		data->left_fork = i + 1;
		if (i == 0)
			data->right_fork = data->params.num_of_ph;
		else
			data->right_fork = i;
		pthread_create(&data->phil[i].thread, NULL, &func, data);
		usleep(50);
		i++;
	}
//	pthread_mutex_destroy(&data->print);
}

int 	main(int argc, char **argv)
{
	t_data		data;

	if (parser(&data, argc, argv))
		print_error("Arguments are not valid", 1);
	processing(&data);
//	print_string(&data);

	return (0);
}
