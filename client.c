/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omerfarukonal <omerfarukonal@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/09 13:30:00 by 42_login          #+#    #+#             */
/*   Updated: 2026/08/09 15:19:49 by omerfarukon      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minitalk.h"

static volatile t_client_state	g_client;

static void	handle_ack(int signal, siginfo_t *info, void *context)
{
	(void)signal;
	(void)context;
	if (!info || info->si_pid <= 0
		|| info->si_pid == (pid_t)g_client.server_pid)
		g_client.ack = 1;
}

static void	install_ack_handler(void)
{
	struct sigaction	action;

	action.sa_sigaction = handle_ack;
	action.sa_flags = SA_SIGINFO;
	if (sigemptyset(&action.sa_mask) == -1
		|| sigaddset(&action.sa_mask, SIGUSR1) == -1
		|| sigaddset(&action.sa_mask, SIGUSR2) == -1
		|| sigaction(SIGUSR1, &action, NULL) == -1)
		ft_error("Error: could not install signal handler.\n");
}

static void	wait_for_ack(void)
{
	int	loops;

	loops = 0;
	while (!g_client.ack && loops < ACK_MAX_LOOPS)
	{
		usleep(ACK_WAIT_US);
		loops++;
	}
	if (!g_client.ack)
		ft_error("Error: server acknowledgment timed out.\n");
}

static void	send_byte(unsigned char byte)
{
	int	bit;
	int	signal;

	bit = 7;
	while (bit >= 0)
	{
		signal = SIGUSR1;
		if (byte & (1 << bit))
			signal = SIGUSR2;
		g_client.ack = 0;
		if (kill((pid_t)g_client.server_pid, signal) == -1)
			ft_error("Error: could not send signal to server.\n");
		wait_for_ack();
		bit--;
	}
}

int	main(int argc, char **argv)
{
	int	index;

	if (argc != 3)
		ft_error("Usage: ./client <server_pid> <message>\n");
	g_client.server_pid = ft_parse_pid(argv[1]);
	if (g_client.server_pid <= 0)
		ft_error("Error: invalid server PID.\n");
	install_ack_handler();
	index = 0;
	while (argv[2][index])
	{
		send_byte((unsigned char)argv[2][index]);
		index++;
	}
	send_byte('\0');
	return (EXIT_SUCCESS);
}
