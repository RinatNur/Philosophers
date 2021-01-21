#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H

# include <stdlib.h>
# include <unistd.h>
# include <stdio.h>
# include <pthread.h>
# include <sys/time.h> //gettimeofday

# define LEFT(n, num_of_ph) ((n - 1 + num_of_ph) % num_of_ph)
# define RIGHT(n, num_of_ph) ((n + 1) % num_of_ph)
# define FORK " has taken a fork\n"
# define EAT " is eating\n"
# define SLEEP " is sleeping\n"
# define THINK " is thinking\n"
# define DIE " died\n"

typedef pthread_mutex_t	t_mutex;

typedef struct		s_params{
	int 	num_of_ph;
	int 	time_to_die;
	int 	time_to_eat;
	int 	time_to_sleep;
	int 	num_of_eating_times;
}					t_params;

typedef struct		s_phil{
	int				index;
	t_mutex			mutex;
	pthread_t		thread;
	long int 		last_eating;
	long int 		action_time;
}					t_phil;

typedef struct		s_data{
	t_params		params;
	t_phil			*phil;
	t_mutex			print;
	int 			left_fork;
	int 			right_fork;
	long int 		start_time;

}					t_data;

int				atoi_mini(int *nbr, char *str);
long int		get_time(void);
size_t			ft_strlen(const char *s);
void			print_2d_mas(char **arr);
void			print_error(char *str, int code);
void			ft_putnbr_fd(long int n, int fd);
ssize_t			ft_write(int fd, const void *buf);

#endif //PHILOSOPHERS_H
