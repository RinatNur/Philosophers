#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H

# include <stdlib.h>
# include <unistd.h>
# include <stdio.h>
# include <string.h> // for memset
# include <pthread.h>
# include <sys/time.h> //gettimeofday

# define LEFT(n, num_of_ph) ((n - 1 + num_of_ph) % num_of_ph)
# define RIGHT(n, num_of_ph) ((n + 1) % num_of_ph)
//# define FORK " has taken a left fork\n"
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

typedef struct		s_data{
	t_params		params;
	t_mutex			print;
	t_mutex			*fork_mutex;
	long int 		start_time;
	int 			is_dead;
}					t_data;

typedef struct		s_phil{
	t_data			*data;
	int				index;
	pthread_t		thread;
	int 			remain_eating_times;
	long int 		last_eating;
	int 			is_eating;
	int 			left_fork;
	int 			right_fork;
}					t_phil;


int				atoi_mini(int *nbr, char *str);
long int		get_time(void);
size_t			ft_strlen(const char *s);
void			print_error(char *str, int code);
void			print_action(t_phil *all, char *str);
void			print_action_dead(t_phil *all, char *str);
void			ft_putnbr_fd(long int n, int fd);
ssize_t			ft_write(int fd, const void *buf);
void			true_sleep(long start, long time_to_sleep);

#endif //PHILOSOPHERS_H
