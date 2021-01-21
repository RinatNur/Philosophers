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

void 	*routine(t_data *data)
{
	for (long long i = 0; i < 1; i++)
	{
		pthread_mutex_lock(&data->print);
		int j = 0;
		while (j < 3)
		{
			write(1, "---------------\n", 16);
			j++;
			sleep(1);
		}
		pthread_mutex_unlock(&data->print);
	}

}

void 	print_string(t_data *data)
{
	long int 	tmp;

	tmp = get_time();
	ft_putnbr_fd(tmp, 1);
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
	ft_putnbr_fd((data->left_fork + 1), 1);
	ft_write(1, str);
	pthread_mutex_unlock(&data->print);
}

void 	*func(void *data)
{
	t_data		*all;
	int 		left;
	int 		right;
	int 		num_of_phil;
	all = (t_data *)data;
	num_of_phil =
	left = all->left_fork;
	right = all->right_fork;
//	while (1)
//	{
//		pthread_mutex_lock(&all->phil[left].mutex);
	int i = 0;
	while (i < all->params.num_of_ph)
	{
		ft_putnbr_fd(all->phil[i].index, 1);
		write(1, "\n", 1);
		i++;
	}
	write(1, "-------------------", 10);
	write(1, "\n", 1);

	all->phil->action_time = get_time();
		print_action(all, all->phil->index, FORK, all->phil->action_time);//TODO number of phil
//		pthread_mutex_lock(&all->phil[right].mutex);
//		all->phil->action_time = get_time();
//		print_action(all, all->phil->index, FORK, all->phil->action_time);
//		pthread_mutex_unlock(&all->phil[left].mutex);
//		pthread_mutex_unlock(&all->phil[right].mutex);
//	}


	return (NULL);
}

void 	processing(t_data *data)
{
	int 			i;

	i = 0;
	if (!(data->phil = (t_phil *)malloc(sizeof (t_phil) * data->params.num_of_ph)))
		print_error("Memory not allocated", 3);
	while (i < data->params.num_of_ph)
	{
		data->phil[i].index = i;
		data->phil[i].last_eating = get_time();
		pthread_mutex_init(&data->phil[i].mutex , NULL);
		i++;
	}
	pthread_mutex_init(&data->print, NULL);
	data->start_time = get_time();

	i = 0;
//	while (i < data->params.num_of_ph)
//	{
//		ft_putnbr_fd(data->phil[i].index, 1);
//		write(1, "\n", 1);
//		i++;
//	}
	while (i < data->params.num_of_ph)
	{
		data->left_fork = LEFT(i, data->params.num_of_ph);
		data->right_fork = RIGHT(i, data->params.num_of_ph);
		pthread_create(&data->phil[i].thread, NULL, &func, data);
		usleep(50);
		i++;
	}


//	i = 0;
//	while (i < data->params.num_of_ph)
//	{
//		data->phil[i].index = i;
//		data->phil[i].last_eating = get_time();
//		pthread_join(&data->phil[i].mutex , NULL);
//		i++;
//	}
	pthread_mutex_destroy(&data->print);
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
