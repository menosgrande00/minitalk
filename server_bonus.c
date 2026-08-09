/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server_bonus.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omerfarukonal <omerfarukonal@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/09 13:30:00 by 42_login          #+#    #+#             */
/*   Updated: 2026/08/09 15:19:23 by omerfarukon      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minitalk_bonus.h"

static volatile t_server_state	g_server;

static void	reset_state(pid_t client_pid)
{
	g_server.client_pid = client_pid;
	g_server.byte = 0;
	g_server.bits = 0;
}

static int	process_bit(int signal)
{
	unsigned char	output;

	g_server.byte = (unsigned char)(g_server.byte << 1);
	if (signal == SIGUSR2)
		g_server.byte |= 1;
	g_server.bits++;
	if (g_server.bits != 8)
		return (SIGUSR1);
	if (g_server.byte == '\0')
	{
		(void)write(STDOUT_FILENO, "\n", 1);
		reset_state(0);
		return (SIGUSR2);
	}
	output = (unsigned char)g_server.byte;
	(void)write(STDOUT_FILENO, &output, 1);
	g_server.byte = 0;
	g_server.bits = 0;
	return (SIGUSR1);
}

static void	handle_signal(int signal, siginfo_t *info, void *context)
{
	int		acknowledgment;
	pid_t	sender_pid;

	(void)context;
	sender_pid = (pid_t)g_server.client_pid;
	if (info && info->si_pid > 0)
		sender_pid = info->si_pid;
	if (sender_pid <= 0)
		return ;
	if (g_server.client_pid != sender_pid)
		reset_state(sender_pid);
	acknowledgment = process_bit(signal);
	if (kill(sender_pid, acknowledgment) == -1)
		reset_state(0);
}

static void	install_handlers(void)
{
	struct sigaction	action;

	action.sa_sigaction = handle_signal;
	action.sa_flags = SA_SIGINFO;
	if (sigemptyset(&action.sa_mask) == -1
		|| sigaddset(&action.sa_mask, SIGUSR1) == -1
		|| sigaddset(&action.sa_mask, SIGUSR2) == -1
		|| sigaction(SIGUSR1, &action, NULL) == -1
		|| sigaction(SIGUSR2, &action, NULL) == -1)
		ft_error("Error: could not install signal handlers.\n");
}

int	main(void)
{
	install_handlers();
	ft_putnbr_fd((int)getpid(), STDOUT_FILENO);
	ft_putstr_fd("\n", STDOUT_FILENO);
	while (1)
		pause();
	return (EXIT_SUCCESS);
}
