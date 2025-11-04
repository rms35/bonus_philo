#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/time.h>
# include <sys/wait.h>
# include <string.h>
# include <stdint.h>
# include <limits.h>
# include <semaphore.h>
# include <sys/stat.h>
# include <fcntl.h>
# include <errno.h>

# define ERR_MSG "Arguments expected: number_of_philosophers, time_to_die,\
 time_to_eat, time_to_sleep [number_of_times_each_philosopher_must_eat]\n"
# define COLL_MSG "All philosophers are collected\n"
# define FORK 1
# define EAT 2
# define SLEEP 3
# define THINK 4
# ifndef NAME_MAX
#  define NAME_MAX 255
# endif

/* Table with the arguments passed to the program */
typedef struct s_table
{
	int				n_philos;
	__useconds_t	t_to_die;
	__useconds_t	t_to_eat;
	__useconds_t	t_to_sleep;
	int				n_to_eat;
}	t_table;

/* Structure relevant to each fhilosopher */
typedef	struct s_philo
{
	int		philo_id;
	t_table	*table;
	long	last_meal_ms;
	long	start_ms;
	int		meals_eaten;
	sem_t	*forks;
	sem_t	*printer;
	sem_t	*die;
	sem_t	*seats;
}	t_philo;

/* Utils and parsing */
void	*ft_calloc(size_t nmemb, size_t size);
int		ft_atoi(char *s);
int		ft_parse(t_table *data, char **argv);
int		ft_strlen(char *s);

/* Freeing allocated memory and liberating resources */
void	ft_close_sems(t_philo **philos);
void	ft_free_when_creating(t_philo **philos, sem_t *forks, t_table *table);
void	ft_free_philos(t_philo **philos);
void	ft_free_child(t_philo **philo);
void	ft_close_forks(sem_t *forks, int child);

/* Time relative */
int		ft_usleep(long ms, t_philo *philo);
long	ft_get_time(void);

/* Simulation */
int						ft_start_sim(t_philo **philos);
static int				ft_philo(t_philo **philo, int n_philo, sem_t *die);
int						ft_wait_philos(int n_philos);


/* @brief Checks if any philosopher has died and prints this philosopher's dead
 if true. */
/* @return 1 if a philosopher has died, else 0 */
static inline int	ft_check_dead(t_philo *philo)
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
static inline int	ft_sems_wait(t_philo *philo)
{
	if (sem_wait(philo->seats) < 0)
		return (write(2,"Error: sem_wait\n",16), 1);
	if (ft_check_dead(philo))
		return (sem_post(philo->seats), 1);
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
		return (write(2,"Error: sem_wait\n",16));		
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
static inline int	ft_takeforks(t_philo *philo)
{
	int		philo_id;
	long	start_t;
	int		dead;

	dead = 0;
	philo_id = philo->philo_id;
	start_t = ft_get_time();
	if (ft_sems_wait(philo))
		return (1);
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
static inline int	ft_sleep(t_philo *philo)
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
static inline int	ft_eat(t_philo *philo)
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

#endif