#include "philo_bonus.h"

/* @brief Waits with waitpid for each philosopher. Probably will need a kill option
for inevitables deadlocks (with sem_t *printer) */
/* @return To be implemented */
int	ft_wait_philos(int n_philos)
{
	int	i;
	int	status;

	i = 1;
	while (i <= n_philos)
	{
		waitpid(-1, &status, 0);
		i++;
	}
	return (1);
}