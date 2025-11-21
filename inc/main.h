/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ndo-vale <ndo-vale@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/25 13:47:48 by ndo-vale          #+#    #+#             */
/*   Updated: 2025/03/10 14:45:10 by ndo-vale         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MAIN_HPP
# define MAIN_HPP

# define WRONG_USAGE_MSG "Wrong usage!"
# define BINDING_ERROR_MSG "Bind of socket to address failed."

# include <iostream>
# include <netinet/in.h> // For everything related to sockets
# include <poll.h>

extern int g_shutdown;

# endif