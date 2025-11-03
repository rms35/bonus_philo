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
	long	elapsed;
	long	remaining;

	start = ft_get_time();
	while (1)
	{
		if (ft_check_dead(philo))
			return (1);
		elapsed = ft_get_time() - start;
		if (elapsed >= ms)
			break ;
		remaining = ms - elapsed;
		if (remaining > 200)
			usleep(200 * 1000);
		else
			usleep(remaining * 1000);
	}
	return (0);
}