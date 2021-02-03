/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jheat <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/31 19:00:53 by jheat             #+#    #+#             */
/*   Updated: 2021/01/31 19:01:11 by jheat            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void 			unlink_sem()
{
	sem_unlink("g_forks");
	sem_unlink("g_print");
}

long int		get_time(void)
{
	struct timeval		tv;
	long int			timestamp;

	gettimeofday(&tv, NULL);
	timestamp = tv.tv_sec * 1000 + tv.tv_usec / 1000;
	return (timestamp);
}

size_t			ft_strlen(const char *s)
{
	size_t		i;

	i = 0;
	while (s[i] != '\0')
		i++;
	return (i);
}

int				atoi_mini(int *nbr, char *str)
{
	if (*str == '\0')
		return (1);
	*nbr = 0;
	while (*str && '0' <= *str && *str <= '9')
	{
		*nbr = *nbr * 10 + *str - '0';
		str++;
	}
	if (*str != '\0' || *nbr == 0)
		return (1);
	return (0);
}

void			ft_putnbr_fd(long int n, int fd)
{
	char		sym;

	if (n > 9)
		ft_putnbr_fd(n / 10, fd);
	sym = n % 10 + '0';
	write(fd, &sym, 1);
}
