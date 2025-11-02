#include "philo_bonus.h"


/* @brief Opens the named semaphores for die, printer and seats, then
initializes them in each philo */
/* @return 0 on succes, 1 if any sem_open failed (and liberates all 
resources, incluiding de t_philo * array) */
static int	ft_init_sem(t_philo **philos, sem_t *forks)
{
	sem_t	*printer;
	sem_t	*die;
	sem_t	*seats;
	int		i;

	die = sem_open("/die", O_CREAT | O_EXCL, 0644, 1);
	seats = sem_open("/seats", O_CREAT, 0644, (*philos)->table->n_philos / 2);
	printer = sem_open("/printer", O_CREAT | O_EXCL, 0644, 1);
	i = 0;
	while (i < philos[0]->table->n_philos)
	{
		philos[i]->die = die;
		philos[i]->seats = seats;
		philos[i]->printer = printer;
		philos[i]->forks = forks;
		i++;
	}
	if (die == SEM_FAILED || seats == SEM_FAILED || printer == SEM_FAILED)
		return (ft_free_philos(philos), write(2, "Error: sem_open\n", 17), 1);
	return (0);
}

/* Creates an array of [number of philosophers] t_philo *philo and initializes
each one of them */
/* @return The pointer to the array on success, NULL on failure, and 
liberates resources  */
static t_philo	**ft_create_philos(t_table *table, sem_t *forks)
{
	t_philo	**philos;
	int		n_philos;
	int		i;

	if (!table || !forks)
		return (NULL);
	philos = (t_philo **)ft_calloc(table->n_philos, sizeof(t_philo *));
	if (!philos)
	{
		ft_close_forks(forks, 0);
		write(2, "Error: malloc\n", 14);
		return (free(table), NULL);
	}
	i = 0;
	while (i < table->n_philos)
	{
		philos[i] = (t_philo *)ft_calloc(1, sizeof(t_philo));
		if (!philos[i])
			return (ft_free_when_creating(philos, forks, table), NULL);
		philos[i]->table = table;
		i++;
	}
	if (ft_init_sem(philos, forks))
		return (NULL);
	return (philos);
}

int	main(int argc, char **argv)
{
	t_table	*table;
	sem_t *forks;
	t_philo	**philos;

	if (argc < 5 || argc > 6)
		return (write(2, ERR_MSG, 128), 1);
	table = (t_table *)ft_calloc(1, sizeof(t_table));
	if (!table)
		return (write(2, "Error: malloc\n", 14), 1);
	if (!ft_parse(table, argv))
		return (free(table), 1);
	forks = sem_open("forks",  O_CREAT | O_EXCL, 0644, table->n_philos);
	if ((forks) == SEM_FAILED)
		return (ft_close_forks(forks, 0), free(table), write(2, "Error: sem_open\n", 16), 1);
	philos = ft_create_philos(table, forks);
	if (!philos)
		return (1);
	ft_start_sim(philos);
}
