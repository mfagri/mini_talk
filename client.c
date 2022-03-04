/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfagri <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/28 17:20:13 by mfagri            #+#    #+#             */
/*   Updated: 2022/01/05 19:05:01 by mfagri           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minitalk.h"

void	sft(int signum)
{
	if (signum == SIGUSR1)
		ft_putstr("\nwell done\n");
}

void	binary(int pid, char *s)
{
	int	i;
	int	bit;

	i = 0;
	if (!s)
		exit(1);
	while (1)
	{
		bit = -1;
		while (++bit < 9)
		{
			if (s[i] & (128 >> bit))
			{
				if (kill(pid, SIGUSR1) == -1)
					exit(1);
			}
			else
				if (kill(pid, SIGUSR2) == -1)
					exit(1);
			usleep(300);
		}
		if (!s[i])
			break ;
		i++;
	}
}

int	main(int ac, char **av)
{
	int	pid;

	if (ac != 3 || av[2][0] == '\0')
	{
		write(1, "error\n", 6);
		exit(1);
	}
	signal(SIGUSR1, sft);
	pid = ft_atoi(av[1]);
	if (pid == -1)
	{
		write(1, "error\n", 6);
		exit(1);
	}
	binary(pid, av[2]);
}
