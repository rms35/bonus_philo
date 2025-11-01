#include "philo_bonus.h"

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

size_t	ft_intlen(long n)
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

char	*ft_get_semname(int n)
{
	char	*name;
	int		len;
	int		i;

	if (n < 0)
		return (NULL);
	name = (char *)ft_calloc(NAME_MAX - 2, sizeof(char));
	if (!name)
		return (write(2, "Error: malloc\n", 15), NULL);
	name[0] = '/';
	name[1] = 's';
	i = 1;
	len = ft_intlen(n);
	while (i < len + 1 && i < (NAME_MAX - 4))
	{
		ft_itoa(name + 2, n);
		i++;
	}
	return (name);
}