/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Webserv.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nsouza-o <nsouza-o@student.42porto.com     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/08 22:51:06 by ndo-vale          #+#    #+#             */
/*   Updated: 2025/03/20 17:07:48 by nsouza-o         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Webserv.hpp"

Webserv::Webserv(int argc, char** argv)
    : _configFile(argc, argv), _portsAm(0){}
Webserv::~Webserv(){}

void    Webserv::setup(void)
{
    _configFile.run();
   
    port_vector ports = _configFile.getPorts();
    _portsAm = ports.size();
    
    
    // Create a listening socket per port
    for (port_vector::const_iterator it = ports.begin(); it != ports.end(); it++) {
        // Create and setup socket
        int listenSocket = socket(AF_INET, SOCK_STREAM, 0);
        fcntl(listenSocket, F_SETFL, O_NONBLOCK);
        int option = 1;
        setsockopt(listenSocket, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, 
                    &option, sizeof(option));
        // Create and setup socket address
        struct sockaddr_in  serv_addr;
        serv_addr.sin_family = AF_INET;
        serv_addr.sin_addr.s_addr = INADDR_ANY;
        serv_addr.sin_port = htons(*it); //Add the current port of the iteration
        
        int bindStatus = bind(listenSocket, (struct sockaddr*)&serv_addr,
                                sizeof(serv_addr));
        if (bindStatus < 0){
            throw (std::runtime_error(BINDING_ERROR_MSG));
        }
        listen(listenSocket, 1);
        
        // Setup the socket to be handled by poll()
        struct pollfd   listenPollfd;
        listenPollfd.fd = listenSocket;
        listenPollfd.events = POLLIN | POLLOUT; //Because screw this subject
        listenPollfd.revents = 0;
        _pollSockets.push_back(listenPollfd);
    }
}

void    Webserv::run(void)
{
    try {
        while (g_shutdown == false) {
            poll(_pollSockets.data(), _pollSockets.size(), 200);
            _takeCareOfClientSockets();
            _takeCareOfListenSockets();
        } 
    } catch (std::exception& e) {
        std::cerr << "Some shit went wrong. " << e.what() << std::endl;
    }
}

/* PRIVATE */
void Webserv::_takeCareOfClientSockets(void)
{
    for (std::list<Client>::iterator client = _clients.begin(); client != _clients.end(); ) {
        int clientIndex = _portsAm + std::distance(_clients.begin(), client);
        client->updateSocketFlags(_pollSockets[clientIndex].revents);
        client->handle();
    
        if (client->shouldCloseConnection()) {
            close(_pollSockets[clientIndex].fd);
            _pollSockets.erase(_pollSockets.begin() + clientIndex);
            client = _clients.erase(client);
        } else {
            ++client;
        }
    } 
}

void    Webserv::_takeCareOfListenSockets()
{
    for (size_t i = 0; i < _portsAm; i++) { //Only go through the listen sockets from the pollSockets (the first _portsAm amout of sockets in that vector)
        if (_pollSockets[i].revents & POLLIN) {
            struct sockaddr_in  cli_addr;
            socklen_t           cli_len = sizeof(cli_addr);
            struct pollfd       cliSock;
            cliSock.fd = accept(_pollSockets[i].fd, (struct sockaddr*)&cli_addr, &cli_len);
            if (cliSock.fd == -1) {
		    std::cerr << "The creation of the connection failed! Ignoring and moving on." << std::endl;
		    continue ;
	    }
	    cliSock.events = POLLIN | POLLOUT;
            cliSock.revents = 0;
            _pollSockets.push_back(cliSock);
	
    		static int id = 0;
            Client  client(cliSock.fd, _configFile, id++);
            _clients.push_back(client);
        }
    } 
}
