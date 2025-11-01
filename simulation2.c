#include "philo_bonus.h"

void	ft_set_dead(sem_t *die)
{
	if (sem_unlink("/die") < 0 && errno != ENOENT)
		write(2, "Error: sem_unlink\n", 16);
}

int	ft_takeforks(t_philo *philo)
{
	int		philo_id;
	long	current_t;
	int		dead;

	dead = 0;
	philo_id = philo->philo_id;
	if (sem_wait(philo->seats) < 0)
		return (write(2,"Error: sem_wait\n",16), 1);
	if (sem_wait(philo->forks) < 0)
		return (write(2,"Error: sem_wait\n",16), 1);
	if (sem_wait(philo->forks) < 0)
	{
		sem_post(philo->forks);
		return (write(2,"Error: sem_wait\n",16), 1);
	}
	if (sem_wait(philo->printer) < 0)
		write(2,"Error: sem_wait\n", 16);
	current_t = ft_get_time();
	if ((current_t - philo->last_meal_ms >= philo->table->t_to_die) || !ft_check_dead())
		printf("%ld ms %d has taken a fork\n", current_t - philo->start_ms, philo_id + 1);
	else
	{
		printf("%ld ms %d died\n", current_t - philo->start_ms, philo_id + 1);
		ft_set_dead(philo->die);
		dead = 1;
	}
	if (sem_post(philo->printer) < 0)
		write(2,"Error: sem_post\n", 16);
	if (sem_post(philo->forks) < 0)
		write(2, "Error: sem_post\n", 16);
	if (sem_post(philo->forks) < 0)
		write(2, "Error: sem_post\n", 16);
	if (sem_post(philo->seats) < 0)
		write(2, "Error: sem_post\n", 16);
	return (dead);
}