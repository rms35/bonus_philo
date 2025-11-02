#include "philo_bonus.h"

/* @brief Releases resources when philosopher initialization fails.
Frees the shared table, closes/unlinks the forks semaphore, and destroys
every `t_philo` node that was already allocated. */
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

/* @brief Closes the forks semaphore and unlinks it on the parent process. */
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
				write(2, "Error: sem_unlink\n", 19);
		}
	}
	return ;
}

/* @brief Frees all child-process resources.
Closes the printer/seats semaphores, releases the table copy and every
`t_philo`, and closes the shared forks semaphore without unlinking. */
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

/* @brief Closes and unlinks the shared semaphores held by the parent. */
/* Frees `philos[0]->printer` and `philos[0]->seats` handles plus `/forks`. */
void	ft_close_sems(t_philo **philos)
{
	if (!philos || !philos[0])
		return ;
	if (philos[0]->printer && sem_close(philos[0]->printer) < 0)
		write(2, "Error: sem_close\n", 17);
	if (philos[0]->seats && sem_close(philos[0]->seats) < 0)
		write(2, "Error: sem_close\n", 17);
	if (philos[0]->printer && sem_unlink("/printer") < 0)
		write(2, "Error: sem_unlink\n", 19);
	if (philos[0]->seats && sem_unlink("/seats") < 0)
		write(2, "Error: sem_unlink\n", 19);
	ft_close_forks((*philos)->forks, 0);

}

/* @brief Fully frees the philosophers array and shared table
in the parent. Closes and unlinks semaphores via `ft_close_sems`,
releases the table, and frees every `t_philo` entry and the
array wrapper. */
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
