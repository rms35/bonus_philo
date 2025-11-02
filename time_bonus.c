# include "philo_bonus.h"

/* @brief Gets the current time since Epoch in miliseconds */
/* @return The (long)time in miliseconds or -1 on error*/
long	ft_get_time(void)
{
	struct timeval	*tv;
	unsigned int	time;

	tv = (struct timeval *)ft_calloc(1, sizeof(struct timeval));
	if (!tv)
		return (-1);
	if (gettimeofday(tv, NULL))
		return (free(tv), -1);
	time = (tv->tv_sec * 1000) + (tv->tv_usec  / 1000);
	free(tv);
	return (time);
}

/* @brief Substitute of usleep, checking for philosophers deads every 200ms */
/* @return 0 if everything went ok, 1 if a philosopher died */
int	ft_usleep(long ms, t_philo *philo)
{
	long	start;
	int		dead;

	start = ft_get_time();
	dead = 0;
	while (1)
	{
		if (ft_check_dead(philo))
		{
			dead = 1;
			break ;
		}
		if (ft_get_time() - start >= ms)
			break ;
		if (ms > 200)
			usleep(200);
		else
		{
			usleep(ms);
			break ;
		}
	}
	return (dead);
}