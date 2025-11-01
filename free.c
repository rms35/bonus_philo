#include "philo_bonus.h"

void	ft_free_forks(t_forks *fs)
{
	int	i;
	int	n_philos;

	i = 0;
	n_philos = fs->n_philos;
	while (i < n_philos)
	{
		printf("freeing names[%d] = %s, forks[%d] = %p\n", i, fs->names[i], i, fs->forks[i]);
		if (fs->forks[i])
		{
			sem_close(fs->forks[i]);
			if (sem_unlink(fs->names[i]) < 0)
				write(2, "Error: sem_unlink\n", 19);
		}
		free(fs->names[i]);
		fs->names[i] = NULL;
		i++;
	}
	free(fs->names);
	free(fs->forks);
	free(fs);
	return ;
}

void	ft_free_philo(t_philo *philo)
{
	if (!philo)
		return ;
	ft_free_forks(philo->forks);
	free(philo->table);
	return ;
}

void	ft_free_philos(t_philo **philos)
{
	int	i;
	int	n_philos;

	if (!philos)
		return ;
	i = 0;
	n_philos = 0;
	sem_close(philos[i]->printer);
	if (sem_unlink("/printer") < 0)
		write(2, "Error: sem_unlink\n", 19);
	if (philos[i])
	{
		printf("philos[%d] = %p, table = %p\n", i, philos[i], philos[i]->table);
		n_philos = philos[0]->table->n_philos;
		ft_free_forks((*philos)->forks);
		free((*philos)->table);
		(*philos)->forks = NULL;
		(*philos)->table = NULL;
	}
	while (i < n_philos)
	{
		free(philos[i]);
		i++;
	}
	free(philos);
	return ;
}