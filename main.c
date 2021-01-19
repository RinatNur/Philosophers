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

void 	*func()
{

}

void 	processing(t_data *data)
{
	int 			i;

	i = 0;
	if (!(data->process = (t_process *)malloc(sizeof (t_process) * data->params.num_of_ph)))
		print_error("Memory not allocated", 3);
	while (i < data->params.num_of_ph)
	{
		data->process[i].index = i;
		data->process[i].last_eating = get_time();
		pthread_mutex_init(&data->process[i].mutex , NULL);
		i++;
	}
	pthread_mutex_init(&data->print, NULL);
	data->start_time = get_time();


	i = 0;
	while (i < data->params.num_of_ph)
	{
		pthread_create(&data->process[i].philosopher, NULL, (void *)(&func), NULL);
	}


	i = 0;
	while (i < data->params.num_of_ph)
	{
		data->process[i].index = i;
		data->process[i].last_eating = get_time();
		pthread_mutex_init(&data->process[i].mutex , NULL);
		i++;
	}
	pthread_mutex_destroy(&data->print);
}

int 	main(int argc, char **argv)
{
	t_data		data;

	if (parser(&data, argc, argv))
		print_error("Arguments are not valid", 1);
	processing(&data);
	print_string(&data);

	return (0);
}
