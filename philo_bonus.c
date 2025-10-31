#include "philo_bonus.h"

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
		printf("n = %d\n", n);
		fs->names[n] = ft_get_semname(n);
		fs->forks[n] = sem_open(fs->names[n],  O_CREAT | O_EXCL, 0644, n);
		printf("names[%d] = %s, forks[%d] = %p\n", n, fs->names[n], n, fs->forks[n]);
		if (!(fs->names[n]) || (fs->forks[n]) == SEM_FAILED || n == 5)
			return (ft_free_forks(fs), NULL);
		n--;
	}
	return (fs);
}

int	main(int argc, char **argv)
{
	t_table	*table;
	t_forks *forks;

	if (argc < 5 || argc > 6)
		return (write(2, ERR_MSG, 128), 1);
	table = (t_table *)ft_calloc(1, sizeof(t_table));
	if (!table)
		return (write(2, "Error: malloc\n", 15), 1);
	if (!ft_parse(table, argv))
		return (free(table), 1);
	forks = (t_forks *)ft_calloc(1, sizeof(t_forks));
	forks->n_philos = table->n_philos;
	forks = ft_init_forks(forks);
	if (!forks)
		return (free(table), 1);
	ft_free_forks(forks);
	free(table);
}
