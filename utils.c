/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omerfarukonal <omerfarukonal@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/09 13:30:00 by 42_login          #+#    #+#             */
/*   Updated: 2026/08/09 15:19:17 by omerfarukon      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minitalk.h"

void	ft_putstr_fd(const char *text, int fd)
{
	int	length;

	length = 0;
	while (text[length])
		length++;
	(void)write(fd, text, length);
}

void	ft_putnbr_fd(int number, int fd)
{
	char	digit;

	if (number >= 10)
		ft_putnbr_fd(number / 10, fd);
	digit = (number % 10) + '0';
	(void)write(fd, &digit, 1);
}

pid_t	ft_parse_pid(const char *text)
{
	int	result;
	int	digit;

	result = 0;
	if (!text || !text[0])
		return (0);
	while (*text)
	{
		if (*text < '0' || *text > '9')
			return (0);
		digit = *text - '0';
		if (result > (INT_MAX - digit) / 10)
			return (0);
		result = (result * 10) + digit;
		text++;
	}
	return ((pid_t)result);
}

void	ft_error(const char *message)
{
	ft_putstr_fd(message, STDERR_FILENO);
	exit(EXIT_FAILURE);
}
