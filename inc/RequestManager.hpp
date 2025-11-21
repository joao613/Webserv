/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestManager.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nsouza-o <nsouza-o@student.42porto.com     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/27 13:52:36 by ndo-vale          #+#    #+#             */
/*   Updated: 2025/03/08 16:02:30 by nsouza-o         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUEST_HANDLER_HPP
# define REQUEST_HANDLER_HPP

# include "StateMachine.hpp"
# include "Socket.hpp"
# include "ConfigFile.hpp"
# include "RequestParser.hpp"
# include "RequestPerformer.hpp"
# include "ResponseSender.hpp"
# include "HttpRequest.hpp"
# include "HttpResponse.hpp"
# include "CGIHandler.hpp"

class RequestManager
{
public:
    typedef HttpResponse::code_t    code_t;
    
    RequestManager(Socket& socket, ConfigFile& configFile);
    ~RequestManager();

    void    handle(void);
    bool    isDone(void) const;
    bool    shouldCloseConnection(void) const;
private:
    enum State {
        RECV_HEADER,
        RECV_BODY,
        CGI_PROCESS,
        SEND_RESPONSE,
        STATE_AM
    };
    StateMachine<State> _stateMachine;

    typedef void (RequestManager::*state_function)();
    std::map<State, state_function>  _stateFunctionsMap;

    enum ErrorSeverity {
        NO_ERROR,
        CONSUME_AND_ANSWER,
        ANSWER_AND_CLOSE,
        CLOSE_IMMEDIATELY,
        ERROR_SEVERITY_AM
    };
    
    Socket&             _socket;
    ServerSettings      _serverSettings;
    

    RequestParser       _requestParser;
    RequestPerformer    _requestPerformer;
    
    ResponseSender      _responseSender;
    CGIHandler          _cgiHandler;
    

    HttpRequest			_request;
	HttpResponse		_response;

    bool	_handlingComplete;
		void	_setHandlingComplete(bool value);
		bool	_getHandlingComplete(void) const;
	bool	_closeConnection;
		void	_setCloseConnection(bool value);
		bool	_getCloseConnection(void) const;
    
    void    _recvHeader(void);
    void    _recvBody(void);
    void    _cgiProcess(void);
    void    _sendResponse(void);

    void            _checkAndActOnErrors(void);
    ErrorSeverity   _getErrorSeverity(code_t statusCode);
};


#endif