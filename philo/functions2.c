/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   functions2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abenba <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/19 14:19:32 by abenba            #+#    #+#             */
/*   Updated: 2025/07/19 14:19:33 by abenba           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	create_philos(void)
{
	pthread_t	monitor_id;
	pthread_t	watcher_id;
	int			i;

	monitor_id = 0;
	watcher_id = 0;
	i = 0;
	while (i < data()->number_of_philosophers)
	{
		data()->philos[i].last_eat = get_time_ms();
		pthread_create(&data()->philos[i].id, NULL, &routine,
			&data()->philos[i]);
		i++;
	}
	pthread_create(&monitor_id, NULL, &monitor, NULL);
	pthread_create(&watcher_id, NULL, watcher, NULL);
	pthread_join(watcher_id, NULL);
	pthread_join(monitor_id, NULL);
}

void	init_philo(void)
{
	int (i) = 0;
	while (i < data()->number_of_philosophers)
	{
		data()->philos[i].checked = 0;
		data()->philos[i].meals_eaten = 0;
		data()->philos[i].seat = i;
		i++;
	}
}

void	init_mutexs(void)
{
	int (i) = 0;
	pthread_mutex_init(&data()->printing, NULL);
	pthread_mutex_init(&data()->eating_count, NULL);
	pthread_mutex_init(&data()->death, NULL);
	pthread_mutex_init(&data()->last_meal, NULL);
	while (i < data()->number_of_philosophers)
	{
		pthread_mutex_init(&data()->forks[i], NULL);
		i++;
	}
}

int	starts(void)
{
	data()->died = 0;
	data()->philos = malloc(sizeof(t_philos) * data()->number_of_philosophers);
	if (data()->philos == NULL)
		return (1);
	data()->forks = malloc(sizeof(pthread_mutex_t)
		*data()->number_of_philosophers);
	if (data()->forks == NULL)
		return (1);
	init_philo();
	init_mutexs();
	return (0);
}

long	get_time_ms(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return (tv.tv_sec * 1000 + tv.tv_usec / 1000);
}
