#include "philo_bonus.h"

int	main()
{
	char	*s;
	int t = 123123123;
	s = ft_get_semname(t);
	printf("s = %s\n", s);
	free(s);
}