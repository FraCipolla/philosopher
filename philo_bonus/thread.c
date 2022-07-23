/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   thread.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcipolla <mcipolla@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/18 18:18:55 by mcipolla          #+#    #+#             */
/*   Updated: 2022/07/18 18:18:55 by mcipolla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

void	*ft_monitor(void *philo)
{
	int		tmp;
	t_philo	*ph;

	ph = philo;
	while (1)
	{
		tmp = ft_time() - ph->rules->start;
		if (tmp - ph->strv > ph->rules->time_death)
		{
			sem_wait(ph->rules->msg);
			printf("%lld %d died\n", ft_time() - ph->rules->start, ph->id);
			sem_post(ph->rules->dead);
			exit(0);
		}
	}
	return (0);
}

void	ft_take_forks(t_philo *ph)
{
	sem_wait(ph->rules->fork);
	ft_philo_msg(ph->rules, ph->id, "has taken a fork");
	sem_wait(ph->rules->fork);
	ft_philo_msg(ph->rules, ph->id, "has taken a fork");
}

void	*ft_meal(t_philo *ph)
{
	ph->rules->start = ft_time();
	ph->strv = ft_time() - ph->rules->start;
	pthread_create(&ph->death, NULL, ft_monitor, ph);
	if (ph->id % 2 == 0)
		my_sleep(ph->rules->time_eat);
	while (1)
	{
		ft_take_forks(ph);
		ft_philo_msg(ph->rules, ph->id, "is eating");
		ph->n_eat++;
		if (ph->rules->ac == 6)
			if (ph->n_eat == ph->rules->must_eat)
				sem_post(ph->rules->finish);
		ph->strv = ft_time() - ph->rules->start;
		my_sleep(ph->rules->time_eat);
		sem_post(ph->rules->fork);
		sem_post(ph->rules->fork);
		ft_philo_msg(ph->rules, ph->id, "is sleeping");
		my_sleep(ph->rules->time_sleep);
		ft_philo_msg(ph->rules, ph->id, "is thinking");
	}
}

void	ft_must_eat(t_rules *ru)
{
	int		i;

	i = 0;
	while (i < ru->n_ph)
	{
		sem_wait(ru->finish);
		i++;
	}
	i = 0;
	sem_post(ru->dead);
	exit(0);
}

void	ft_thread(t_rules *rules)
{
	int		i;
	t_philo	*philo;

	philo = rules->philo;
	if (fork() == 0)
		ft_must_eat(rules);
	i = 0;
	while (i < rules->n_ph)
	{
		philo->rules->pid[i] = fork();
		if (philo->rules->pid[i] == 0)
		{
			ft_meal(&rules->philo[i]);
			return ;
		}
		i++;
	}
	kill_all(rules);
	exit(0);
}
