#include "philo_bonus.h"
static t_philo	**ft_create_philos(t_table *table, t_forks *forks)
{
	t_philo	**philos;
	sem_t	*printer;
	int		i;

	if (!table || !forks)
		return (NULL);
	philos = (t_philo **)ft_calloc(table->n_philos, sizeof(t_philo *));
	if (!philos)
		return (write(2, "Error: malloc\n", 14), ft_free_forks(forks, 0), free(table), NULL);
	printer = sem_open("/printer", O_CREAT | O_EXCL, 0644, 1);
	if (printer == SEM_FAILED)
		return (free(philos), ft_free_forks(forks, 0), free(table), write(2, "Error: sem_open1\n", 16), NULL);
	i = 0;
	while (i < table->n_philos)
	{
		philos[i] = (t_philo *)ft_calloc(1, sizeof(t_philo));
		if (!philos[i])
			return (write(2, "Error: malloc\n", 15), ft_free_philos(philos), NULL);
		philos[i]->table = table;
		philos[i]->forks = forks;
		philos[i]->printer = printer;
		i++;
	}
	return (philos);
}

static t_forks	*ft_init_forks(t_forks *fs)
{
	int		n;

	n = fs->n_philos - 1;
	fs->forks = (sem_t **)ft_calloc(n + 2, sizeof(sem_t *));
	fs->names = (char **)ft_calloc(n + 2, sizeof(char *));
	if (!fs->forks || !fs->names)
		return (NULL);
	while (n >= 0)
	{
		fs->names[n] = ft_get_semname(n + 1);
		if (!fs->names)
			return (ft_free_forks(fs, 0), write(2, "Error: malloc\n", 14), NULL);
		fs->forks[n] = sem_open(fs->names[n],  O_CREAT | O_EXCL, 0644, n);
		if ((fs->forks[n]) == SEM_FAILED)
			return (ft_free_forks(fs, 0), write(2, "Error: sem_open2\n", 16), NULL);
		if (!(fs->names[n]))
			return (ft_free_forks(fs, 0), write(2, "Error: malloc\n", 14), NULL);
		n--;
	}
	return (fs);
}

int	main(int argc, char **argv)
{
	t_table	*table;
	t_forks *forks;
	t_philo	**philos;

	if (argc < 5 || argc > 6)
		return (write(2, ERR_MSG, 128), 1);
	table = (t_table *)ft_calloc(1, sizeof(t_table));
	if (!table)
		return (write(2, "Error: malloc\n", 14), 1);
	if (!ft_parse(table, argv))
		return (free(table), 1);
	forks = (t_forks *)ft_calloc(1, sizeof(t_forks));
	if (!forks)
		return (free(table), write(2, "Error: malloc\n", 14), 1);
	forks->n_philos = table->n_philos;
	forks = ft_init_forks(forks);
	if (!forks)
		return (free(table), 1);
	philos = ft_create_philos(table, forks);
	if (!philos)
		return (1);
	ft_start_sim(philos);
}
