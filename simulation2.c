#include "philo_bonus.h"

/* @brief Checks if any philosopher has died and returns 1 if true. */
/* @return 1 if a philosopher has died, else 0 */
int	ft_check_dead(void)
{
	sem_t *s;

	errno = 0;
	s = sem_open("/die", 0);
	if (s == SEM_FAILED && errno == ENOENT)
		return (1);
	if (s != SEM_FAILED)
		sem_close(s) < 0;
	return (0);
}


/* @brief Semwaits for seats, two forks and printer, writing
corresponding error messages */
/* @return 0 if everithing went ok, 1 if any sem_wait error */
int	ft_sems_wait(t_philo *philo)
{
	if (ft_check_dead())
		return (1);
	if (sem_wait(philo->seats) < 0)
		return (write(2,"Error: sem_wait\n",16), 1);
	if (sem_wait(philo->forks) < 0)
	{
		sem_post(philo->seats);
		return (write(2,"Error: sem_wait\n",16), 1);
	}
	if (sem_wait(philo->forks) < 0)
	{
		sem_post(philo->forks);
		sem_post(philo->seats);
		return (write(2,"Error: sem_wait\n",16), 1);
	}
	if (sem_wait(philo->printer) < 0)
	{
		sem_post(philo->forks);
		sem_post(philo->forks);
		sem_post(philo->seats);
		return (write(2,"Error: sem_wait\n", 16), 1);
	}
	return (0);
}

/* @brief Semposts for seats, two forks and printer, writing
corresponding error messages */
/* @return 0 if everithing went ok, 1 if any sem_post error */
int	ft_sems_post(t_philo *philo)
{
	int	ret;

	ret = 0;
	if (sem_post(philo->printer) < 0)
	{
		write(2,"Error: sem_post\n", 16);
		ret = 1;
	}
	if (sem_post(philo->forks) < 0)
	{
		write(2,"Error: sem_post\n", 16);
		ret = 1;
	}
	if (sem_post(philo->forks) < 0)
	{
		write(2,"Error: sem_post\n", 16);
		ret = 1;
	}
	if (sem_post(philo->seats) < 0)
	{
		write(2,"Error: sem_post\n", 16);
		ret = 1;
	}
	return (ret);
}

/* Simulation of taking two forks (with sem_wait) */
/* @return 0 if everything went ok, 1 if any semaphore operation failed
or checked that a philosopher died */
int	ft_takeforks(t_philo *philo)
{
	int		philo_id;
	long	start_t;
	int		dead;

	dead = 0;
	philo_id = philo->philo_id;
	start_t = ft_get_time();
	if (ft_sems_wait(philo))
		return (1);
	if ((start_t - philo->last_meal_ms <= philo->table->t_to_die) || !ft_check_dead())
		printf("%ld ms %d has taken a fork\n", start_t - philo->start_ms, philo_id + 1);
	else
	{
		printf("%ld ms %d died\n", start_t - philo->start_ms, philo_id + 1);
		if (sem_unlink("/die") < 0 && errno != ENOENT)
			write(2, "Error: sem_unlink\n", 19);
		dead = 1;
	}
	if (dead || sem_post(philo->printer) < 0 || sem_post(philo->seats) < 0)
	{
		write(2,"Error: sem_post\n", 16);
		dead = 1;
	}
	return (dead);
}
/* @brief Simulation of sleeping time_to_sleep miliseconds */
/* @return 0 if everything went ok, 1 if any semaphore operation failed
or checked that a philosopher died  */
int	ft_sleep(t_philo *philo)
{
	int				dead;
	int				philo_id;
	unsigned int	time;

	dead = 0;
	philo_id = philo->philo_id + 1;
	time = ft_get_time();
	if (ft_check_dead())
		return (1);
	if (sem_wait(philo->printer) < 0)
		write(2, "Error: sem_wait\n", 16);
	printf("%ld ms %d is sleeping\n", time - philo->start_ms, philo_id);
	if (ft_usleep(philo->table->t_to_sleep))
		dead = 1;
	if (dead != 0 && sem_post(philo->printer) < 0)
		write(2, "Error: sem_post\n", 16);
	return (dead);
}

/* @brief Simulation of eating time_to_eat miliseconds */
/* @return 0 if everything went ok, 1 if any semaphore operation failed
or checked that a philosopher died  */
int	ft_eat(t_philo *philo)
{
	if (ft_check_dead())
		return (1);
	if (sem_wait(philo->printer) < 0)
		return (write(2, "Error: sem_wait\n", 16), 1);
	if (ft_usleep(philo->table->t_to_eat))
		return (1);
	if (sem_post(philo->printer) < 0)
		return (write(2, "Error: sem_wait\n", 16), 1);
	ft_sems_post();
}