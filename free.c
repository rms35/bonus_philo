#include "philo_bonus.h"

void	ft_close_forks(sem_t *forks, int child)
{
	int	i;

	i = 0;
	if (forks)
		{
			if (sem_close(forks) < 0)
				write(2, "Error: sem_close\n", 17);
			if (!child)
			{
				if (sem_unlink("/forks") < 0)
					write(2, "Error: sem_unlink\n", 18);
			}
		}
	return ;
}

void	ft_free_child(t_philo **philos)
{
	int	i;
	int	n_philos;

	if (!philos)
		return ;
	i = 0;
	n_philos = 0;
	if (sem_close(philos[i]->printer) < 0)
		write(2, "Error: sem_close\n", 17);
	if (sem_close(philos[i]->seats) < 0)
		write(2, "Error: sem_close\n", 17);
	if (philos[i])
	{
		n_philos = philos[0]->table->n_philos;
		ft_close_forks((*philos)->forks, 1);
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

void	ft_free_philos(t_philo **philos)
{
	int	i;
	int	n_philos;

	if (!philos)
		return ;
	i = 0;
	n_philos = 0;
	if (sem_close(philos[i]->printer) < 0)
		write(2, "Error: sem_close\n", 17);
	if (sem_close(philos[i]->seats) < 0)
		write(2, "Error: sem_close\n", 17);
	if (sem_unlink("/printer") < 0)
		write(2, "Error: sem_unlink\n", 18);
	if (sem_unlink("/seats") < 0)
		write(2, "Error: sem_unlink\n", 18);
	if (philos[i])
	{
		n_philos = philos[0]->table->n_philos;
		ft_close_forks((*philos)->forks, 0);
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