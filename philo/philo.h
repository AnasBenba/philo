/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abenba <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/19 14:26:30 by abenba            #+#    #+#             */
/*   Updated: 2025/07/19 14:26:32 by abenba           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/time.h>
# include <unistd.h>

typedef struct s_philos
{
	pthread_t		id;
	int				seat;
	long			last_eat;
	int				meals_eaten;
	int				checked;
}					t_philos;

typedef struct s_data
{
	int				died;
	int				number_of_philosophers;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				times_each_must_eat;
	int				status;
	pthread_mutex_t	*forks;
	t_philos		*philos;
	long			start;
	pthread_mutex_t	printing;
	pthread_mutex_t	last_meal;
	pthread_mutex_t	eating_count;
	pthread_mutex_t	death;
	pthread_mutex_t	table;
	int				odd;
	int				philosophers_done;

}			t_data;

t_data		*data(void);
void		check_args(char **args);
int			is_num(char *arg);
int			ft_atoi(const char *str);
size_t		ft_strlen(const char *c);
int			ft_isdigit(int c);
long		get_time_ms(void);
int			starts(void);
void		init_mutexs(void);
void		init_philo(void);
void		create_philos(void);
void		*watcher(void *arg);
void		*monitor(void *arg);
void		*routine(void *arg);
void		think(t_philos *philos);
void		ft_sleep(t_philos *philos);
void		eat(t_philos *philos);
void		print_action(t_philos *philo, char *msg);
void		ft_usleep(long duration);
int			is_died(void);
void		put_forks(int right, int left, t_philos *philos);
void		take_forks(t_philos *philos);
void		lock_forks(int right, int left, t_philos *philos);
int			check_the_checked(void);
int			checked(int i);

#endif
