/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   message.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcipolla <mcipolla@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/18 18:18:08 by mcipolla          #+#    #+#             */
/*   Updated: 2022/07/18 18:18:10 by mcipolla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

long long	ft_time(void)
{
	struct timeval	time;

	gettimeofday(&time, NULL);
	return ((time.tv_sec * 1000) + (time.tv_usec / 1000));
}

void	my_sleep(long long time)
{
	long long	tmp;

	tmp = ft_time();
	usleep(time * 1000 - 20000);
	while (ft_time() < tmp + time)
		continue ;
}

void	ft_philo_msg(t_philo *ph, int id, char *str)
{
	pthread_mutex_lock(&ph->rules->lock);
	printf("%lld ", ft_time() - ph->rules->start);
	printf("%d %s\n", id, str);
	pthread_mutex_unlock(&ph->rules->lock);
}
