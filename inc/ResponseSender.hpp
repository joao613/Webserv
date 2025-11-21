/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ResponseSender.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ndo-vale <ndo-vale@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/27 20:12:50 by ndo-vale          #+#    #+#             */
/*   Updated: 2025/03/21 08:43:38 by ndo-vale         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RESPONSE_SENDER
# define RESPONSE_SENDER

# define DELIMITOR "\r\n"

# include "AMessageHandler.hpp"
# include "Socket.hpp"

class ResponseSender: public AMessageHandler
{
public:
    typedef Socket::data_container_t data_t;

    ResponseSender(HttpRequest& request, HttpResponse& response);
    ~ResponseSender();

    data_t    getMessageToSend(size_t byteLimit);

private:
    std::ifstream   _file;
    bool            _headersSent; 

    std::string _generateResponseHeader(void);
};

#endif
