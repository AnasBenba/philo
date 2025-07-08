#include "philo.h"

t_data	*data(void)
{
	static t_data	d;

	return (&d);
}

void	free_all(void)
{
	int	i;

	i = 0;
	while (i < data()->number_of_philosophers)
		pthread_mutex_destroy(&data()->forks[i++]);
	pthread_mutex_destroy(&data()->printing);
	pthread_mutex_destroy(&data()->eating_count);
	pthread_mutex_destroy(&data()->death);
	pthread_mutex_destroy(&data()->last_meal);
	pthread_mutex_destroy(&data()->table);
	free(data()->philos);
	free(data()->forks);
}

int main(int ac, char **av)
{
    int (i) = 0;
    if (ac != 5 && ac != 6)
        return (1);
    check_args(av);
    data()->start = get_time_ms();
    if (starts() == 1)
        return (1);
    else
    {
        create_philos();
		while (i < data()->number_of_philosophers)
		{
			pthread_join(data()->philos[i].id, NULL);
			i++;
		}
    }
    free_all();
	return (0);
}