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

# define ERR_MSG "Arguments expected: number_of_philosophers, time_to_die, time_to_eat, time_to_sleep [number_of_times_each_philosopher_must_eat]\n"
# define COLL_MSG "All philosophers are collected\n"
# define FORK 1
# define EAT 2
# define SLEEP 3
# define THINK 4
# ifndef NAME_MAX
#  define NAME_MAX 255
# endif

typedef struct s_table
{
	int				n_philos;
	__useconds_t	t_to_die;
	__useconds_t	t_to_eat;
	__useconds_t	t_to_sleep;
	int				n_to_eat;
}	t_table;
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

int		ft_takeforks(t_philo *philo);
int		ft_check_dead(void);
int		ft_start_sim(t_philo **philos);
void	ft_free_philos(t_philo **philos);
void	ft_free_child(t_philo **philo);
void	ft_close_forks(sem_t *forks, int child);
void	*ft_calloc(size_t nmemb, size_t size);
void	ft_free_list(t_philo **start);
void	ft_usleep(long ms);
void	ft_set_dead(sem_t *die);
int		ft_atoi(char *s);
int		ft_strlen(char *s);
int		ft_parse(t_table *data, char **argv);
int		ft_set_timer(t_philo *node);
long	ft_time_printer(t_philo *philo, int act);
long	ft_get_time(void);
int		ft_philo(t_philo **philo, int n_philo, sem_t *die);

#endif