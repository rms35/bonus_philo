#include "philo_bonus.h"
int	ft_philo(t_philo **philos, int n_philo)
{
	t_philo	*philo;

	philo = philos[n_philo];
	philo->philo_id = n_philo;
	sem_wait(philo->printer);
	printf("Hello, I'm philo[%d]\n", n_philo);
	sleep(1);
	sem_post(philo->printer);
	ft_free_child(philos);
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


	if (!philos)
		return (1);
	i = 0;
	n_philos = philos[i]->table->n_philos;
	while (i < n_philos)
	{
		pid = fork();
		if (pid == 0)
		{
			ft_philo(philos, i);
		}
		else
			i++;
	}
	ft_wait_philos(n_philos);
	ft_free_philos(philos);
}