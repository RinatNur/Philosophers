#include "philosophers.h"

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


long int 	get_time(void)
{
	struct timeval		tv;
	long int 			timestamp;

	gettimeofday(&tv, NULL);
	timestamp = tv.tv_sec * 1000 + tv.tv_usec / 1000;
//	printf("timestamp = %ld\n", timestamp);
	return (timestamp);
}

void 	print_error(char *str, int code)
{
	write (1, str, ft_strlen(str));
	exit (code);
}

int		atoi_mini(int *nbr, char *str)
{
	if (*str == '\0')
		return (1);
	*nbr = 0;
	while (*str && '0' <= *str && *str <= '9')
	{
		*nbr = *nbr * 10 + *str - '0';
		*str++;
	}
	if (*str != '\0' || *nbr == 0)
		return (1);
	return(0);
}

void		ft_putnbr_fd(long int n, int fd)
{
	char		sym;
	char 		*str;
	int 		i;
	long int 		tmp;

	i = 1;
	tmp = n;
	while(tmp > 9)
	{
		tmp /= 10;
		i++;
	}
	str = malloc(i + 1);
	str[i] = '\0';
	if (n > 9)
		ft_putnbr_fd(n / 10, fd);
	sym = n % 10 + '0';
	write(fd, &sym, 1);
}

