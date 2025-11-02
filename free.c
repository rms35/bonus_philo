#include "philo_bonus.h"

void	ft_free_when_creating(t_philo **philos, sem_t *forks, t_table *table)
{
	if (!philos)
		return ;
	ft_close_forks(forks, 0);
	free(table);
	table = NULL;
	forks = NULL;
	ft_free_philos(philos);
	write(2, "Error: malloc\n", 15);
	return ;
}

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

void	ft_close_sems(t_philo **philos)
{
	if (!philos || !philos[0])
		return ;
	if (philos[0]->printer && sem_close(philos[0]->printer) < 0)
		write(2, "Error: sem_close\n", 17);
	if (philos[0]->seats && sem_close(philos[0]->seats) < 0)
		write(2, "Error: sem_close\n", 17);
	if (philos[0]->die && sem_close(philos[0]->die) < 0)
		write(2, "Error: sem_close\n", 17);
	if (philos[0]->die && sem_unlink("/die") < 0)
		write(2, "Error: sem_unlink\n", 18);
	if (philos[0]->printer && sem_unlink("/printer") < 0)
		write(2, "Error: sem_unlink\n", 18);
	if (philos[0]->seats && sem_unlink("/seats") < 0)
		write(2, "Error: sem_unlink\n", 18);
	ft_close_forks((*philos)->forks, 0);

}

void	ft_free_philos(t_philo **philos)
{
	int	i;
	int	n_philos;

	if (!philos)
		return ;
	i = 0;
	n_philos = 0;
	if (philos[0])
	{
		ft_close_sems(philos);
		if (philos[0]->table)
		{
			n_philos = philos[0]->table->n_philos;
			free((*philos)->table);
		}
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