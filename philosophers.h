#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H

# include <stdlib.h>
# include <unistd.h>
# include <stdio.h>
# include <pthread.h>

typedef struct		s_params{
	int 	num_of_ph;
	int 	time_to_die;
	int 	time_to_eat;
	int 	time_to_sleep;
	int 	num_of_eating_times;
}					t_params;

int		atoi_mini(int *nbr, char *str);

#endif //PHILOSOPHERS_H
