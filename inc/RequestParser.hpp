/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestParser.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ndo-vale <ndo-vale@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/27 19:37:12 by ndo-vale          #+#    #+#             */
/*   Updated: 2025/03/11 15:19:43 by ndo-vale         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUEST_PARSER
# define REQUEST_PARSER

# define DELIMITOR "\r\n"
# define TOKEN_CHARS "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789!#$%&'*+-.^_`|~"

# include "AMessageHandler.hpp"
# include "Socket.hpp"
# include "StateMachine.hpp"
# include "ServerSettings.hpp"
# include "utils.hpp"

class RequestParser: public AMessageHandler
{
public:
    typedef HttpResponse::code_t code_t;
    typedef Socket::data_container_t data_t;
    
    RequestParser(HttpRequest& request, HttpResponse& response,
                    ServerSettings& serverSettings);
    ~RequestParser();
    
    size_t  parse(const data_t& data);
    
private:
    enum State {
        PARSING_REQUEST_LINE,
        PARSING_HEADERS,
        PROCESSING_REQUEST,
        STATE_AM
    };
    StateMachine<State> _stateMachine;

    ServerSettings&     _serverSettings;
    
    std::string         _dataStr;
        void    _setDataStr(std::string dataStr);
        void    _consumefromDataStr(size_t byteAm);
    size_t              _dataConsumed;

    void	_parseRequestLine(void);
	void	_parseHeaders(void);
	void	_processRequest(void);

    code_t	_httpSanitizer(std::string line);
	code_t	_fillInRequestLineInfo(std::string requestLine);
	code_t	_parseHeaderField(std::string headerField);
	void	_abortRequestHandling(code_t statusCode);
	Method	_strToMethod(std::string str);
};

#endif
