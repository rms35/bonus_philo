#include "philo_bonus.h"

/* @brief Executes philosophers actions: taking forks, eating, sleeping
and thinking */
/* @return to be decided upon */
int	ft_philo(t_philo **philos, int n_philo, sem_t *die)
{
	t_philo	*philo;
	int		i;

	i = 1;
	philo = philos[n_philo];
	philos[n_philo]->philo_id = n_philo;
	philos[n_philo]->start_ms = ft_get_time();
	while (1)
	{
		if (philo->table->n_to_eat > 0 && i >= philo->table->n_to_eat)
			break;
		if (ft_takeforks(philo))
			break ;
		if (ft_sleep(philo->table->t_to_sleep, philo))
			break ;
		i++;
	}
	ft_free_child(philos);
	sem_close(die);
	sem_unlink("/die");
	exit(EXIT_SUCCESS);
}

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
		printf("%d philosophers dead\n", i);
		i++;
	}
	return (1);
}

/* @brief Liberates resources when failed memory allocation for pid_t array.
Writes corresponding error messages */
/* @return Always 1, so we can return this function directly */
int	ft_pid_error(t_philo **philos)
{
	write(2, "Error: malloc\n", 14);
	ft_free_philos(philos);
	if (sem_close((*philos)->die) < 0)
		write(2, "Error: sem_close\n", 17);
	return (1);
}

/* @brief Starts the simulation. Creates the pid_t array (probably should do it
in another function, merged with the ft_pid_error function), and forks once for
each philosopher, then waits for each proccess. Liberates all resources of the
father */
/* @return 0 if everything went ok, 1 for any failure */
int	ft_start_sim(t_philo **philos)
{
	int		i;
	int		n_philos;
	pid_t	*pid;

	i = 0;
	n_philos = philos[i]->table->n_philos;
	pid = ft_calloc(n_philos + 1, sizeof(pid_t));
	if (!pid)
		return (ft_pid_error(philos));
	while (i < n_philos)
	{
		pid[i] = fork();
		if (pid[i] < 0)
		{
			write(2, "Error: fork\n", 12);
			break ;
		}
		if (pid[i] == 0)
			ft_philo(philos, i, (*philos)->die);
		i++;
	}
	ft_wait_philos(n_philos);
	if (sem_close((*philos)->die) < 0)
		write(2, "Error: sem_close\n", 17);
	ft_free_pid(pid, n_philos);
	ft_free_philos(philos);
}