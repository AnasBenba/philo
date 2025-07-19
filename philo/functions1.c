/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   functions1.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abenba <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/19 14:19:24 by abenba            #+#    #+#             */
/*   Updated: 2025/07/19 14:19:26 by abenba           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	ft_isdigit(int c)
{
	if (c >= 48 && c <= 57)
		return (1);
	return (0);
}

size_t	ft_strlen(const char *c)
{
	size_t	len;

	len = 0;
	while (c[len])
		len++;
	return (len);
}

int	ft_atoi(const char *str)
{
	long	result;
	int		i;

	result = 0;
	i = 0;
	while (str[i] == ' ' || (str[i] >= 9 && str[i] <= 13))
		i++;
	if (str[i] == '-' || str[i] == '+')
	{
		if (str[i] == '-')
			return (0);
		i++;
	}
	while (ft_isdigit(str[i]))
	{
		result = result * 10 + (str[i] - '0');
		if (result > __INT_MAX__)
			return (0);
		i++;
	}
	return ((int)(result));
}

int	is_num(char *arg)
{
	int	i;

	i = 0;
	if (!arg || !*arg)
		return (1);
	if (arg[0] == '-' || arg[0] == '+')
		i++;
	while (arg[i])
	{
		if (!ft_isdigit(arg[i]))
			return (1);
		i++;
	}
	return (0);
}

void	check_args(char **args)
{
	int (i) = 1;
	while (args[i])
	{
		if (is_num(args[i]) || ft_atoi(args[i]) <= 0)
		{
			write(2, "Invalid argument", 17);
			write(2, args[i], ft_strlen(args[i]));
			write(2, "\n", 1);
			exit(EXIT_FAILURE);
		}
		i++;
	}
	data()->odd = 0;
	data()->number_of_philosophers = ft_atoi(args[1]);
	data()->time_to_die = ft_atoi(args[2]);
	data()->time_to_eat = ft_atoi(args[3]);
	data()->time_to_sleep = ft_atoi(args[4]);
	if (args[5] == NULL)
		data()->times_each_must_eat = -1;
	else
		data()->times_each_must_eat = ft_atoi(args[5]);
	if (data()->number_of_philosophers % 2)
		data()->odd = 1;
}
