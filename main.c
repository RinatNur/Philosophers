#include "philosophers.h"

int 	left(int n)
{
	return ((n - 1 + 5 ) % 5);
}

int 	right(int n)
{
	return ((n + 1) % 5);
}

void 	parser()
{

}

size_t		ft_strlen(const char *s)
{
	size_t	i;

	i = 0;
	while (s[i] != '\0')
		i++;
	return (i);
}

void 	print_2d_mas(char **arr)
{
	int i;

	i = 0;
	while (arr[i])
	{
		write (1, arr[i], ft_strlen(arr[i]));
		write (1, "\n", 1);
		i++;
	}
}

long long mails = 0;
pthread_mutex_t mutex;

void 	*routine()
{
	for (long long i = 0; i < 10000000; i++)
	{
		pthread_mutex_lock(&mutex);
		mails++;
		pthread_mutex_unlock(&mutex);
	}

}

int 	main(int argc, char **argv)
{
//	t_params		params;
//	char 			**arguments;
//	int 			i;
//
//	i = 1;
//	arguments = malloc(sizeof(char *) * argc);
//	while (i < argc)
//	{
//		arguments[i] = malloc(ft_strlen(argv[i]) + 1);
//		arguments[i - 1] = argv[i];
//		i++;
//	}
//	arguments[i - 1] = NULL;
//	i = 0;
//	while (arguments[i])
//	{
//
//	}
//	print_2d_mas(arguments);

	int i = 0;
	int count = 0;
	pthread_mutex_init(&mutex, NULL);
	pthread_t th[8];
	for (i; i < 8; i++)
	{
		pthread_create(th + i, NULL, &routine, NULL);
		printf("Thread number %d is started\n", i);
//		pthread_join(th[i], NULL);
//		printf("Thread number %d stoped\n", i);

	}
	for (i = 0; i < 8; i++)
	{
		pthread_join(th[i], NULL);
		write(1,"Thread number %d stoped\n", 14);
		int t = i + '0';
		write(1, &t, 1);
		write(1," stoped\n", 8);

	}
//	pthread_join(t1, NULL);
//	pthread_create(&t2, NULL, &routine, NULL);
//	pthread_join(t2, NULL);
//	pthread_mutex_destroy(&mutex);
	printf("Number of mails: %lld\n", mails);

//	while (i < 10000000)
//	{
//		printf("%d\n", count);
//		count ++;
//		i++;
//	}
	return (0);
}
