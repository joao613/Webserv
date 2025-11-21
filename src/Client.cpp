/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nsouza-o <nsouza-o@student.42porto.com     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/25 14:39:26 by ndo-vale          #+#    #+#             */
/*   Updated: 2025/03/12 17:00:48 by nsouza-o         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

Client::Client(int sockfd, ConfigFile& configFile, int id)
    : _socket(sockfd), _configFile(configFile),
        _activeRequest(NULL), _closeConnection(false), _timeoutTime(60), _id(id)
{
	_lastActionTime = getCurrentTimestamp();
}

Client::Client(const Client& other): _socket(other._socket),
    _configFile(other._configFile), _activeRequest(other._activeRequest),
    _closeConnection(other._closeConnection),
    _timeoutTime(other._timeoutTime), _lastActionTime(other._lastActionTime), _id(other._id)
{
}

Client& Client::operator=(const Client& other)
{
    if (this != &other) {
        _socket = other._socket;
        _activeRequest = other._activeRequest;
        _closeConnection = other._closeConnection;
        _lastActionTime = other._lastActionTime;
	_id = other._id;
    }
    return (*this);
}

Client::~Client()
{
    if (_activeRequest) {
        delete _activeRequest;
    }
}

void    Client::handle(void)
{
    do {
        
        if (_isNewRequestRequired()) {
            _activeRequest = new RequestManager(_socket, _configFile);
        }
        if (_activeRequest) {
            _activeRequest->handle(); //The bulk of the work is done here
            if (_activeRequest->isDone()) {
                _setCloseConnection(_activeRequest->shouldCloseConnection());
                delete _activeRequest;
                _activeRequest = NULL;
            }
        }
        if (_socket.wasActionMade()) {
            _lastActionTime = getCurrentTimestamp();
        } else if (hasTimedOut(_lastActionTime, _timeoutTime)) {
            std::cerr << "The client " << _id << " has timed out." << std::endl;
            _setCloseConnection(true);
        }
    } while (_isNewRequestRequired() && !shouldCloseConnection());
}

void    Client::updateSocketFlags(short revents)
{
    _socket.updateFlags(revents);
}

bool    Client::shouldCloseConnection(void) const
{
    return (_getCloseConnection());
}

void    Client::_setCloseConnection(bool closeConnection) {_closeConnection = closeConnection;}
bool    Client::_getCloseConnection(void) const {
    return (_closeConnection);}

bool    Client::_isNewRequestRequired(void) const
{
    bool    outstandingDataExists = _socket.getRecvStash().size() > 0 
                                || _socket.canRecv();
    return (!_activeRequest && outstandingDataExists);
}
