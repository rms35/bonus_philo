#include "philo_bonus.h"

int	ft_check_dead(void)
{
	sem_t *s;

	errno = 0;
	s = sem_open("/die", 0);
	if (s == SEM_FAILED && errno == ENOENT)
		return (1);
	if (s != SEM_FAILED)
		sem_close(s);
	return (0);
}

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
		if (ft_check_dead())
			break;
		if (philo->table->n_to_eat > 0 && i >= philo->table->n_to_eat)
			break;
		if (ft_takeforks(philo))
			break ;
		i++;
	}
	ft_free_child(philos);
	sem_close(die);
	exit(EXIT_SUCCESS);
}

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

int	ft_start_sim(t_philo **philos)
{
	int		i;
	int		n_philos;
	pid_t	pid;

	i = 0;
	n_philos = philos[i]->table->n_philos;
	while (i < n_philos)
	{
		pid = fork();
		if (pid == 0)
			ft_philo(philos, i, (*philos)->die);
		i++;
	}
	ft_wait_philos(n_philos);
	if (sem_close((*philos)->die) < 0)
		write(2, "Error: sem_close\n", 17);
	if (sem_unlink("/die") < 0)
		write(2, "Error: sem_close\n", 17);
	ft_free_philos(philos);
}