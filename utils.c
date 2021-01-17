#include "philosophers.h"

int		atoi_mini(int *nbr, char *str)
{
	if (*str == '\0')
		return (1);
	*nbr = 0;
	while (*str && '0' <= *str && *str <= '9')
		*nbr = *nbr * 10 + *str - '0';
	if (*str != '\0' || *nbr == 0)
		return (1);
	return(0);
}