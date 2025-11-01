#include "philo_bonus.h"

int	ft_takeforks(t_philo *philo)
{
	int		philo_id;
	long	current_t;

	philo_id = philo->philo_id;	
	if (sem_wait(philo->forks->forks[philo->fork1]) < 0)
		write(2,"Error: sem_wait\n", 16);
	if (sem_wait(philo->forks->forks[philo->fork2]) < 0)
		write(2,"Error: sem_wait\n", 16);
	if (sem_wait(philo->printer) < 0)
		write(2,"Error: sem_wait\n", 16);
	current_t = ft_get_time();
	if ((current_t - philo->last_meal_ms > philo->table->t_to_eat) || ft_check_dead())
		;
	printf("%ld ms %d\n", current_t - philo->start_ms, philo_id + 1);
	if (sem_post(philo->forks->forks[philo->fork2]) < 0)
		write(2, "Error: sem_post\n", 16);
	if (sem_post(philo->forks->forks[philo->fork1]) < 0)
		write(2, "Error: sem_post\n", 16);
}