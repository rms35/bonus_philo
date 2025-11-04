#include "philo_bonus.h"

static void	ft_kill_philo(t_philo **philos, int n_philo, sem_t *die)
{
	t_philo	*philo;

	philo = philos[n_philo];
	philo->philo_id = n_philo + 1;
	if (sem_wait(philo->printer) < 0)
		write(2, "Error: sem_wait\n", 16);
	printf("%ld ms %d died\n", ft_get_time() - philo->start_ms, philo->philo_id);
	if (sem_post(philo->printer) < 0)
		write(2, "Error: sem_wait\n", 16);
	if (sem_unlink("/die") < 0 && errno != ENOENT)
		write(2, "Error: sem_unlink1\n", 19);
	ft_free_child(philos);
	sem_close(die);
	exit(EXIT_SUCCESS);
}

/* @brief Prints when the philosopher starts thinking if no other 
philosofer died */
/* @return 0 on success, 1 if a philosopher died */
static int	ft_think(t_philo *philo)
{
	long	t;
	int		dead;

	t = ft_get_time();
	if (sem_wait(philo->printer) < 0)
		write(2,"Error: sem_wait\n", 16);
	dead = ft_check_dead(philo);
	if (!dead)
		printf("%ld ms %d is thinking\n", t - philo->start_ms, philo->philo_id);
	if (sem_post(philo->printer) < 0)
		write(2,"Error: sem_post\n", 16);
	return (dead);
}

/* @brief Executes philosophers actions: taking forks, eating, sleeping
and thinking */
/* @return to be decided upon */
static int	ft_philo(t_philo **philos, int n_philo, sem_t *die)
{
	t_philo	*philo;
	int		i;

	i = 1;
	philo = philos[n_philo];
	philo->philo_id = n_philo + 1;
	philo->start_ms = ft_get_time();
	philo->last_meal_ms = philo->start_ms;
	while (1)
	{
		if (philo->table->n_to_eat > 0 && i >= philo->table->n_to_eat)
			ft_kill_philo(philos, n_philo, die);
		if (ft_takeforks(philo))
			break ;
		if (ft_eat(philo))
			break ;
		if (ft_sleep(philo))
			break ;
		if (ft_think(philo))
			break ;
		i++;
	}
	ft_free_child(philos);
	sem_close(die);
	if (sem_unlink("/die") < 0 && errno != ENOENT)
		write(2, "Error: sem_unlink2\n", 19);
	exit(EXIT_SUCCESS);
}

/* @brief Starts the simulation. Creates the pid_t array (probably should do it
in another function, merged with the ft_pid_error function), and forks once for
each philosopher, then waits for each proccess. Liberates all resources of the
father */
/* @return 0 if everything went ok, 1 for any failure */
void	ft_start_sim(t_philo **philos)
{
	int		i;
	int		n_philos;
	pid_t	pid;

	i = 0;
	n_philos = philos[i]->table->n_philos;
	while (i < n_philos)
	{
		pid = fork();
		if (pid < 0)
		{
			write(2, "Error: fork\n", 12);
			break ;
		}
		if (pid == 0)
			ft_philo(philos, i, (*philos)->die);
		i++;
	}
	ft_wait_philos(n_philos);
	if (sem_close((*philos)->die) < 0)
		write(2, "Error: sem_close\n", 17);
	ft_free_philos(philos);
}