/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfagri <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/28 17:39:53 by mfagri            #+#    #+#             */
/*   Updated: 2021/12/29 13:28:10 by mfagri           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minitalk.h"

void	fmsg(int pid, int signum)
{
	if (kill(pid, signum) == -1)
		exit(EXIT_FAILURE);
}

void	bta(int signum, siginfo_t *info, void *context)
{
	static int	g_pid = 0;
	static char	c = 0xFF;
	static int	bit = 0;	

	(void) context;
	if (g_pid != info->si_pid)
	{
		c = 0xFF;
		bit = 0;
		g_pid = info->si_pid;
	}
	if (signum == SIGUSR1)
		c |= 0x80 >> bit;
	else if (signum == SIGUSR2)
		c ^= 0x80 >> bit;
	if (bit++ == 8)
	{
		if (!c)
			fmsg(g_pid, SIGUSR1);
		ft_putchar(c);
		bit = 0;
		c = 0xFF;
	}
}

int	main(int ac, char **av)
{
	struct sigaction	sa;

	if (ac != 1 || av[1] != NULL)
	{
		write(1, "error\n", 6);
		exit(1);
	}
	sa.sa_sigaction = bta;
	sa.sa_flags = SA_SIGINFO;
	sigaction(SIGUSR1, &sa, NULL);
	sigaction(SIGUSR2, &sa, NULL);
	ft_putstr("PID : ");
	ft_putnbr(getpid());
	ft_putchar('\n');
	while (1)
		pause();
	return (0);
}
