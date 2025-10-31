#include "philo_bonus.h"

int	ft_start_sim(t_table *table, t_forks *forks)
{
	t_philo	*philo;

	if (!table || !forks)
		return (1);
	philo = (t_philo *)ft_calloc(1, sizeof(t_philo));
	if (!philo)
		return (write(2, "Error: malloc\n", 15), 1);
	philo->table = table;
	philo->forks = forks;
	
}