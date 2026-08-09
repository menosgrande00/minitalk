/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minitalk.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omerfarukonal <omerfarukonal@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/09 13:30:00 by 42_login          #+#    #+#             */
/*   Updated: 2026/08/09 15:19:29 by omerfarukon      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINITALK_H
# define MINITALK_H

# include <limits.h>
# include <signal.h>
# include <stdlib.h>
# include <sys/types.h>
# include <unistd.h>

# define ACK_WAIT_US 50
# define ACK_MAX_LOOPS 100000

typedef struct s_client_state
{
	sig_atomic_t	ack;
	sig_atomic_t	server_pid;
}	t_client_state;

typedef struct s_server_state
{
	sig_atomic_t	client_pid;
	sig_atomic_t	byte;
	sig_atomic_t	bits;
}	t_server_state;

void	ft_putstr_fd(const char *text, int fd);
void	ft_putnbr_fd(int number, int fd);
pid_t	ft_parse_pid(const char *text);
void	ft_error(const char *message);

#endif
