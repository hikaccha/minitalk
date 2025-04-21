/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hichikaw <hichikaw@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/01 10:00:00 by hikaru            #+#    #+#             */
/*   Updated: 2025/04/21 15:43:01 by hichikaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minitalk.h"

static void	send_bit(int pid, int bit)
{
	if (bit == 1)
	{
		if (kill(pid, SIGUSR1) == -1)
		{
			ft_putstr_fd("Error sending signal\n", 2);
			exit(1);
		}
	}
	else
	{
		if (kill(pid, SIGUSR2) == -1)
		{
			ft_putstr_fd("Error sending signal\n", 2);
			exit(1);
		}
	}
}

void	send_char(int pid, char c)
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

void	send_string(int pid, char *str)
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
	int	server_pid;

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
	send_string(server_pid, argv[2]);
	return (0);
}
