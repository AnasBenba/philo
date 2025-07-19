/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   functions.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abenba <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/19 14:19:15 by abenba            #+#    #+#             */
/*   Updated: 2025/07/19 14:19:18 by abenba           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	is_died(void)
{
	int (i) = 0;
	pthread_mutex_lock(&data()->death);
	i = data()->died;
	pthread_mutex_unlock(&data()->death);
	return (i);
}

void	ft_usleep(long duration)
{
	long	start;

	start = get_time_ms();
	while (get_time_ms() - start < duration && !is_died())
		usleep(500);
}

void	print_action(t_philos *philo, char *msg)
{
	long	timestamp;

	timestamp = get_time_ms() - data()->start;
	pthread_mutex_lock(&data()->printing);
	if (!is_died())
		printf("%ld %d %s\n", timestamp, philo->seat + 1, msg);
	pthread_mutex_unlock(&data()->printing);
}

void	lock_forks(int right, int left, t_philos *philos)
{
	(void)philos;
	if (philos->seat + 1 == data()->number_of_philosophers - 1)
	{
		pthread_mutex_lock(&data()->forks[left]);
		pthread_mutex_lock(&data()->forks[right]);
		print_action(philos, "take left fork");
		print_action(philos, "take right fork");
	}
	else
	{
		pthread_mutex_lock(&data()->forks[right]);
		pthread_mutex_lock(&data()->forks[left]);
		print_action(philos, "take right fork");
		print_action(philos, "take left fork");
	}
}
