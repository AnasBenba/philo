#include "philo.h"

int	is_died(void)
{
	int	(i) = 0;
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
	if (philos->seat + 1 == data()->number_of_philosophers- 1)
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
	if (philos->checked == 0 && philos->meals_eaten >= data()->times_each_must_eat)
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
	int			i;

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
				print_action(&data()->philos[i], "died");
				pthread_mutex_lock(&data()->death);
				data()->died = 1;
				pthread_mutex_unlock(&data()->death);
				return (NULL);
			}
			i++;
		}
	}
	return (NULL);
}

void create_philos(void)
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

void    init_philo(void)
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

void init_mutexs(void)
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

int starts(void)
{
    data()->died = 0;
    data()->philos = malloc(sizeof(t_philos) * data()->number_of_philosophers);
    if (data()->philos == NULL)
        return (1);
    data()->forks = malloc(sizeof(pthread_mutex_t) * data()->number_of_philosophers);
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

void check_args(char **args)
{
    int (i) = 1;
    while (args[i])
    {
        if (is_num(args[i]) || ft_atoi(args[i]) <= 0)
        {
            write (2, "Invalid argument", 17);
            write (2, args[i], ft_strlen(args[i]));
            write (2, "\n", 1);
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