#include "philo_bonus.h"

/* @brief Checks if any philosopher has died and prints this philosopher's dead
 if true. */
/* @return 1 if a philosopher has died, else 0 */
int	ft_check_dead(t_philo *philo)
{
	sem_t *s;
	long	time;

	errno = 0;
	time = ft_get_time();
	s = sem_open("/die", 0);
	if (s == SEM_FAILED && errno == ENOENT)
	{
		printf("%ld ms %d died\n", time - philo->start_ms, philo->philo_id);
		return (1);
	}
	if (s != SEM_FAILED)
	{
		if (sem_close(s) < 0)
			write(2, "Error: sem_close\n", 17);
	}
	return (0);
}


/* @brief Semwaits for seats, two forks and printer, writing
corresponding error messages */
/* @return 0 if everithing went ok, 1 if any sem_wait error */
static int	ft_sems_wait(t_philo *philo)
{
	if (sem_wait(philo->seats) < 0)
		return (sem_post(philo->printer), write(2,"Error: sem_wait\n",16), 1);
	if (ft_check_dead(philo))
		return (sem_post(philo->printer), sem_post(philo->seats), 1);
	if (sem_wait(philo->forks) < 0)
	{
		sem_post(philo->printer);
		sem_post(philo->seats);
		return (write(2,"Error: sem_wait\n",16), 1);
	}
	if (sem_wait(philo->forks) < 0)
	{
		sem_post(philo->printer);
		sem_post(philo->forks);
		sem_post(philo->seats);
		return (write(2,"Error: sem_wait\n",16), 1);
	}	
	return (0);
}

/* @brief Sem_posts for seats and two forks, writing
corresponding error messages if necessary */
/* @return 0 if everithing went ok, 1 if any sem_post error */
static int	ft_sems_post(t_philo *philo)
{
	int	ret;

	ret = 0;
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
	if (sem_post(philo->printer) < 0)
	{
		write(2, "Error: sem_post\n", 16);
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
	if (sem_wait(philo->printer) < 0)
		return (ft_sems_post(philo), write(2,"Error: sem_wait\n",16), 1);
	printf("%d meal time = %ld\n", philo->philo_id,  start_t - philo->last_meal_ms);
	dead = ft_check_dead(philo);
	if ((start_t - philo->last_meal_ms <= philo->table->t_to_die) || !dead)
	{
		
		printf("%ld ms %d has taken fork\n", start_t - philo->start_ms, philo_id);
		if (sem_post(philo->printer) < 0)
			write(2,"Error: sem_post\n", 16);
	}
	else
	{
		printf("%ld ms %d died\n", start_t - philo->start_ms, philo_id);
		if (sem_post(philo->printer) < 0)
			write(2, "Error: sem_post\n", 16);	
		if (sem_unlink("/die") < 0 && errno != ENOENT)
			write(2, "Error: sem_unlink\n", 19);
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
	philo_id = philo->philo_id;
	time = ft_get_time();
	if (sem_wait(philo->printer) < 0)
		write(2, "Error: sem_wait\n", 16);
	if (ft_check_dead(philo))
	{
		if (sem_post(philo->printer) < 0)
			write(2, "Error: sem_post\n", 16);
		return (1);
	}
	printf("%d meal time = %ld\n", philo->philo_id,  time - philo->last_meal_ms);
	printf("%ld ms %d is sleeping\n", time - philo->start_ms, philo_id);
	if (sem_post(philo->printer) < 0)
		write(2, "Error: sem_post\n", 16);
	if (ft_usleep(philo->table->t_to_sleep, philo))
	{
		if (sem_unlink("/die") < 0 && errno != ENOENT)
			write(2, "Error: sem_unlink\n", 19);
		dead = 1;
	}
	return (dead);
}

/* @brief Simulation of eating time_to_eat miliseconds */
/* @return 0 if everything went ok, 1 if any semaphore operation failed, 
time between lunches is bigger than time_to_die or checked that a
philosopher died  */
int	ft_eat(t_philo *philo)
{
	long	t;
	int		dead;

	if (sem_wait(philo->printer) < 0)
		return (ft_sems_post(philo), write(2, "Error: sem_wait\n", 16), 1);
	if (ft_check_dead(philo))
	{
		ft_sems_post(philo);
		return (1);
	}
	t = ft_get_time();
	dead = 0;
	printf("%d meal time = %ld\n", philo->philo_id,  t - philo->last_meal_ms);
	if (t - philo->last_meal_ms > philo->table->t_to_die)
	{
		printf("%ld ms %d died\n", t - philo->start_ms, philo->philo_id);
		if (sem_unlink("/die") < 0 && errno != ENOENT)
			write(2, "Error: sem_unlink\n", 18);
		dead = 1;
	}
	else
		printf("%ld ms %d is eating\n", t - philo->start_ms, philo->philo_id);
	ft_sems_post(philo);
	if (dead || ft_usleep(philo->table->t_to_eat, philo))
		return (1);
	philo->last_meal_ms = t;
	return (0);
}