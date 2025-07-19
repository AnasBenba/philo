/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   functions3.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abenba <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/19 14:19:39 by abenba            #+#    #+#             */
/*   Updated: 2025/07/19 14:19:41 by abenba           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	*routine(void *arg)
{
	t_philos	*philo;

	philo = (t_philos *)arg;
	if (data()->number_of_philosophers == 1)
	{
		pthread_mutex_lock(&data()->forks[0]);
		print_action(&data()->philos[0], "take the left fork");
		ft_usleep(data()->time_to_die);
		pthread_mutex_unlock(&data()->forks[0]);
	}
	if ((philo->seat + 1) % 2)
		usleep(1000);
	while (!is_died())
	{
		eat(philo);
		ft_sleep(philo);
		think(philo);
	}
	return (NULL);
}

int	checked(int i)
{
	pthread_mutex_lock(&data()->eating_count);
	if (data()->philos[i].checked)
		return (pthread_mutex_unlock(&data()->eating_count), 1);
	return (pthread_mutex_unlock(&data()->eating_count), 0);
}

int	check_the_checked(void)
{
	int	i;

	i = 0;
	while (i < data()->number_of_philosophers)
	{
		if (!checked(i))
			return (0);
		i++;
	}
	return (1);
}

void	*monitor(void *arg)
{
	int	i;

	(void)arg;
	while (1)
	{
		i = 0;
		while (i < data()->number_of_philosophers)
		{
			if (is_died())
				return (NULL);
			if (data()->times_each_must_eat > 0)
			{
				if (check_the_checked())
				{
					pthread_mutex_lock(&data()->death);
					data()->died = 1;
					pthread_mutex_unlock(&data()->death);
					return (NULL);
				}
			}
			i++;
		}
	}
	return (NULL);
}

void	*watcher(void *arg)
{
	int		i;
	long	last;

	(void)arg;
	while (1)
	{
		i = 0;
		while (i < data()->number_of_philosophers)
		{
			pthread_mutex_lock(&data()->last_meal);
			last = data()->philos[i].last_eat;
			pthread_mutex_unlock(&data()->last_meal);
			if (get_time_ms() - last > data()->time_to_die)
			{
				pthread_mutex_lock(&data()->death);
				data()->died = 1;
				pthread_mutex_unlock(&data()->death);
				printf("%ld %d %s\n", get_time_ms() - data()->start,
					data()->philos[i].seat + 1, "died");
				return (NULL);
			}
			i++;
		}
	}
	return (NULL);
}
