/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ichikawahikaru <ichikawahikaru@student.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/01 10:00:00 by hikaru            #+#    #+#             */
/*   Updated: 2025/08/02 13:47:12 by ichikawahik      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minitalk.h"

static volatile sig_atomic_t	g_bit_count = 0;

static void	reset_buffer(char *buffer, int *byte_index, int *bit_index)
{
	ft_putstr_fd(buffer, 1);
	buffer[0] = '\0';
	*byte_index = 0;
	*bit_index = 0;
}

static void	handle_signal(int signum, siginfo_t *info, void *context)
{
	static char	buffer[4096];
	static int	byte_index = 0;
	static int	bit_index = 0;
	static char	current_char = 0;
	pid_t		client_pid;
	sigset_t	mask;
	sigset_t	oldmask;

	(void)context;
	client_pid = info->si_pid;
	sigemptyset(&mask);
	sigaddset(&mask, SIGUSR1);
	sigaddset(&mask, SIGUSR2);
	sigprocmask(SIG_BLOCK, &mask, &oldmask);
	if (signum == SIGUSR1)
		current_char |= (1 << bit_index);
	bit_index++;
	if (bit_index == 8)
	{
		buffer[byte_index++] = current_char;
		buffer[byte_index] = '\0';
		if (current_char == '\0' || byte_index >= 4095)
			reset_buffer(buffer, &byte_index, &bit_index);
		else
			bit_index = 0;
		current_char = 0;
	}
	kill(client_pid, SIGUSR1);
	sigprocmask(SIG_SETMASK, &oldmask, NULL);
	g_bit_count++;
}

int	main(void)
{
	struct sigaction	sa;

	ft_putstr_fd("Server PID: ", 1);
	ft_putnbr_fd(getpid(), 1);
	ft_putstr_fd("\n", 1);
	sa.sa_sigaction = handle_signal;
	sigemptyset(&sa.sa_mask);
	sigaddset(&sa.sa_mask, SIGUSR1);
	sigaddset(&sa.sa_mask, SIGUSR2);
	sa.sa_flags = SA_SIGINFO | SA_RESTART;
	if (sigaction(SIGUSR1, &sa, NULL) == -1)
	{
		ft_putstr_fd("Error setting up SIGUSR1 handler\n", 2);
		return (1);
	}
	if (sigaction(SIGUSR2, &sa, NULL) == -1)
	{
		ft_putstr_fd("Error setting up SIGUSR2 handler\n", 2);
		return (1);
	}
	while (1)
		pause();
	return (0);
}
