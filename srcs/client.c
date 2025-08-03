/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ichikawahikaru <ichikawahikaru@student.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/01 10:00:00 by hikaru            #+#    #+#             */
/*   Updated: 2025/08/03 14:48:01 by ichikawahik      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minitalk.h"

static volatile sig_atomic_t	g_ack_received = 0;

static void	ack_handler(int signum)
{
	(void)signum;
	g_ack_received = 1;
}

static void	send_bit(int pid, int bit)
{
	int	retries;
	int	wait_count;

	retries = 0;
	while (retries < 10)
	{
		g_ack_received = 0;
		if (bit == 1)
			kill(pid, SIGUSR1);
		else
			kill(pid, SIGUSR2);
		wait_count = 0;
		while (!g_ack_received && wait_count < 1000)
		{
			usleep(100);
			wait_count++;
		}
		if (g_ack_received)
			break ;
		retries++;
		usleep(1000);
	}
}

static void	send_char(int pid, char c)
{
	int	bit;
	int	i;

	i = 0;
	while (i < 8)
	{
		bit = (c >> i) & 1;
		send_bit(pid, bit);
		i++;
		usleep(100);
	}
}

static void	send_string(int pid, char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		send_char(pid, str[i]);
		i++;
	}
	send_char(pid, '\0');
}

int	main(int argc, char **argv)
{
	int					server_pid;
	struct sigaction	sa;

	if (argc != 3)
	{
		ft_putstr_fd("Usage: ./client [server_pid] [string]\n", 2);
		return (1);
	}
	server_pid = ft_atoi(argv[1]);
	if (server_pid <= 0)
	{
		ft_putstr_fd("Invalid PID\n", 2);
		return (1);
	}
	sa.sa_handler = ack_handler;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_RESTART;
	sigaction(SIGUSR1, &sa, NULL);
	send_string(server_pid, argv[2]);
	return (0);
}
