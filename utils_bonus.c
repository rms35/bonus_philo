#include "philo_bonus.h"


/* @brief Calloc implementation */
void *ft_calloc(size_t nmemb, size_t size)
{
	void	*result;
	size_t 	bytes;
	size_t	i;

	if (nmemb == 0 || size == 0)
	{
		result = malloc(0);
		return (result);
	}
	if (nmemb && nmemb > SIZE_MAX / size)
		return (NULL);
	bytes = nmemb * size;
	result = malloc(bytes);
	if (!result)
		return (NULL);
	i = 0;
	while (i < (bytes))
	{
		((char *)result)[i] = '\0';
		i++;
	}
	return (result);
}

/* @brief Strlen implementation, but more secure */
int	ft_strlen(char *s)
{
	int	i;

	i = 0;
	if (!s)
		return (0);
	while (s[i])
		i++;
	return (i);
}

static size_t	ft_intlen(long n)
{
	size_t	len;

	len = 1;
	if (n < 0)
	{
		len++;
		n = -n;
	}
	while (n > 9)
	{
		n = n / 10;
		len++;
	}
	return (len);
}

/* @brief Converts an integer into a string in the passed string */
/* @return The string with the converted integer, NULL if no string
was passed */
char	*ft_itoa(char *str, int n)
{
	size_t	len;
	long	nbr;

	nbr = (long)n;
	len = ft_intlen(n);
	if (!str)
		return (NULL);
	len--;
	if (nbr < 0)
	{
		nbr = -nbr;
		str[0] = '-';
	}
	while (nbr > 9)
	{
		str[len] = (nbr % 10) + 48;
		nbr = nbr / 10;
		len--;
	}
	str[len] = nbr + 48;
	return (str);
}