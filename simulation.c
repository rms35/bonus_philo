#include "philo_bonus.h"

int	ft_check_dead(sem_t *die)
{
	sem_t *s;

	if (sem_wait(die) < 0)
		write(2,"Error: sem_wait\n", 16);
	errno = 0;
	s = sem_open("/die", O_CREAT | O_EXCL, 0644, 1);
	if (s != SEM_FAILED && errno == EEXIST)
	{
		if (sem_close(s) < 0)
			write(2, "Error: sem_close\n", 17);
		if (sem_post(die) < 0)
			write(2, "Error: sem_post\n", 16);
		return (1);
	}
	if (sem_post(die) < 0)
		write(2, "Error: sem_post\n", 16);
	return (0);
}

void	ft_load_philo(t_philo **philos, int n_philo, sem_t *die)
{
	philos[n_philo]->philo_id = n_philo;
	philos[n_philo]->start_ms = ft_get_time();
	philos[n_philo]->fork1 = philos[n_philo]->philo_id;
	if (philos[n_philo]->philo_id % 2)
		philos[n_philo]->fork2 = philos[n_philo]->philo_id + 1;
	else
		philos[n_philo]->fork2 = philos[n_philo]->philo_id - 1;
	philos[n_philo]->die = die;
}

int	ft_philo(t_philo **philos, int n_philo, sem_t *die)
{
	t_philo	*philo;
	int		i;

	i = 1;
	philo = philos[n_philo];
	ft_load_philo(philos, n_philo, die);
	while ((philo->table->n_to_eat && i < philo->table->n_to_eat) || i)
	{

		if (ft_check_dead(die) || i == philo->table->n_to_eat)
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
	sem_t	*die;

	i = 0;
	n_philos = philos[i]->table->n_philos;
	die = sem_open("/die", O_CREAT | O_EXCL, 0644, 1);
	if (die == SEM_FAILED)
		return (ft_free_philos(philos), write(2, "Error: sem_open3\n", 16), 1);
	while (i < n_philos)
	{
		pid = fork();
		if (pid == 0)
			ft_philo(philos, i, die);
		i++;
	}
	ft_wait_philos(n_philos);
	if (sem_close(die) < 0)
		write(2, "Error: sem_close\n", 17);
	if (sem_unlink("/die") < 0)
		write(2, "Error: sem_close\n", 17);
	ft_free_philos(philos);
}