/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   functions4.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abenba <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/19 14:19:46 by abenba            #+#    #+#             */
/*   Updated: 2025/07/19 14:19:48 by abenba           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	take_forks(t_philos *philos)
{
	int	left;
	int	right;

	left = philos->seat;
	right = (philos->seat + 1) % data()->number_of_philosophers;
	lock_forks(right, left, philos);
}

void	put_forks(int right, int left, t_philos *philos)
{
	if (philos->seat == data()->number_of_philosophers - 1)
	{
		pthread_mutex_unlock(&data()->forks[left]);
		pthread_mutex_unlock(&data()->forks[right]);
	}
	else
	{
		pthread_mutex_unlock(&data()->forks[right]);
		pthread_mutex_unlock(&data()->forks[left]);
	}
}

void	eat(t_philos *philos)
{
	int	left;
	int	right;

	if (is_died())
		return ;
	take_forks(philos);
	pthread_mutex_lock(&data()->last_meal);
	philos->last_eat = get_time_ms();
	pthread_mutex_unlock(&data()->last_meal);
	print_action(philos, "is eating");
	philos->meals_eaten++;
	if (philos->checked == 0
		&& philos->meals_eaten >= data()->times_each_must_eat)
	{
		pthread_mutex_lock(&data()->eating_count);
		philos->checked = 1;
		pthread_mutex_unlock(&data()->eating_count);
	}
	ft_usleep(data()->time_to_eat);
	left = philos->seat;
	right = (philos->seat + 1) % data()->number_of_philosophers;
	put_forks(right, left, philos);
}

void	ft_sleep(t_philos *philos)
{
	print_action(philos, "is sleeping");
	ft_usleep(data()->time_to_sleep);
}

void	think(t_philos *philos)
{
	print_action(philos, "is thinking");
	if (data()->odd && (philos->seat + 1) % 2)
	{
		if (!is_died() && data()->time_to_eat > data()->time_to_sleep)
			usleep((data()->time_to_eat - data()->time_to_sleep) * 1000);
		usleep(500);
	}
}
