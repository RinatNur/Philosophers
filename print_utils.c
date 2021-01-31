/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jheat <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/31 18:59:34 by jheat             #+#    #+#             */
/*   Updated: 2021/01/31 18:59:36 by jheat            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

ssize_t		ft_write(int fd, const void *buf)
{
	int		len;
	int		ret;

	len = ft_strlen(buf);
	((ret = write(fd, buf, len)) == -1)
	? print_error("Error in function write", 3) : 0;
	return (ret);
}

void		print_action(t_phil *all, char *str)
{
	long int		time;

	pthread_mutex_lock(&all->data->print);
	time = get_time() - all->data->start_time;
	ft_putnbr_fd(time, 1);
	ft_write(1, " ");
	ft_putnbr_fd((all->left_fork), 1);
	ft_write(1, str);
	if (all->data->is_dead == 1)
		pthread_mutex_lock(&all->data->print);
	pthread_mutex_unlock(&all->data->print);
}

void		print_action_dead(t_phil *all, char *str)
{
	long int		time;

	time = get_time() - all->data->start_time;
	ft_putnbr_fd(time, 1);
	ft_write(1, " ");
	ft_putnbr_fd((all->left_fork), 1);
	ft_write(1, str);
}

void		print_error(char *str, int code)
{
	write(1, str, ft_strlen(str));
	exit(code);
}
