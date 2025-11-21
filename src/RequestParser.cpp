/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestParser.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nsouza-o <nsouza-o@student.42porto.com     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/27 19:56:27 by ndo-vale          #+#    #+#             */
/*   Updated: 2025/03/14 16:00:04 by nsouza-o         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "RequestParser.hpp"

RequestParser::RequestParser(HttpRequest& request, HttpResponse& response,
                            ServerSettings& serverSettings)
    : AMessageHandler(request, response), _stateMachine(STATE_AM, PARSING_REQUEST_LINE),
        _serverSettings(serverSettings), _dataConsumed(0)
{}
RequestParser::~RequestParser(){}

size_t    RequestParser::parse(const data_t& data)
{
    _setDataStr(std::string(data.begin(), data.end()));    
    if (_dataStr.length() > BUFFER_SIZE * 4) {
        _abortRequestHandling(431); 
    }
    State   currentState = _stateMachine.getCurrentState();
    State   previousState = currentState;
    do {
        switch (currentState) {
            case PARSING_REQUEST_LINE:
               _parseRequestLine();
               break;
            case PARSING_HEADERS:
                _parseHeaders();
                break;
            case PROCESSING_REQUEST:
                _processRequest();
                break;
            default:
                std::cerr << "Some unexpected state happened!" << std::endl;
                throw (std::exception());
        };
        previousState = currentState;
        currentState = _stateMachine.getCurrentState();
    } while (currentState != previousState);
    return (_dataConsumed);
}

void    RequestParser::_setDataStr(std::string dataStr)
{
    _dataStr = dataStr;
    _dataConsumed = 0;
}
void    RequestParser::_consumefromDataStr(size_t byteAm)
{
    size_t  amToRemove = std::min(_dataStr.length(), byteAm);
    _dataStr.erase(_dataStr.begin(), _dataStr.begin() + amToRemove);
    _dataConsumed += amToRemove;
}

void	RequestParser::_parseRequestLine()
{
    size_t                          requestLineSize = _dataStr.find(DELIMITOR);

    if (requestLineSize != _dataStr.npos) { //found
        if (requestLineSize > BUFFER_SIZE) { //line too big
            _abortRequestHandling(414);
            return;
        }
        std::string requestLine = _dataStr.substr(0, requestLineSize);
        code_t  parsingResult = _fillInRequestLineInfo(requestLine); //actual parsing is done here
        if (parsingResult != 200) {            
            _abortRequestHandling(parsingResult);
            return;
        } else {
            _consumefromDataStr(requestLineSize + std::string(DELIMITOR).length());
            _stateMachine.advanceState();
        }
    } else if (_dataStr.size() > BUFFER_SIZE) { //stash too big to hold a line (first "if" already checked if line is shorter)
        _abortRequestHandling(414);
        return;
    } else {
        return; //Returns without modification if any of the above isn't true, as it needs to wait for the next read
    }
    
}
void	RequestParser::_parseHeaders(void)
{
    const size_t    delimitorSize = std::string(DELIMITOR).length();
    
    while (_dataStr.compare(0, 2, DELIMITOR) != 0) { // As headers are consumed dinamically, delimitor of header section will be at the beginning
        if (_request.getHeadersSize() > BUFFER_SIZE * 4) {
            _abortRequestHandling(431);
            return;
        }
        size_t  fieldEnd = _dataStr.find(DELIMITOR);
        if (fieldEnd == _dataStr.npos) {
            return; //Header field is not complete, so a new recv must be done before continuing parsing
        }
        std::string field = _dataStr.substr(0, fieldEnd);
        code_t      parsingResult = _parseHeaderField(field);
        if (parsingResult != 200) {
            _abortRequestHandling(parsingResult);
            return;
        }
        _consumefromDataStr(fieldEnd + delimitorSize);
        _request.setHeadersSize(_request.getHeadersSize() + fieldEnd + delimitorSize);
    }
    //Since all conditions inside the while loop have returns, this section is only reached
    // if the end of the header section is reached, so the last delimitor should be cleared and state advanced
    _consumefromDataStr(2);
    _stateMachine.advanceState();
}
void	RequestParser::_processRequest(void)
{
	if (_serverSettings.getLocation().empty()) {
		_abortRequestHandling(404);
		return;
	}
    //Check return code
    int  output = _serverSettings.getReturnCode();
    if (output != -1) { // Means that a return code is present
        code_t  returnCode = static_cast<code_t>(output);
        bool    isRedirectCode = _serverSettings.getReturnCode() >= 300
                                    && _serverSettings.getReturnCode() < 400;
        if (isRedirectCode) {
            _response.addHeader("Location", _serverSettings.getReturnURL());
        }
        _abortRequestHandling(returnCode);
        return;
    }
    if (_serverSettings.getAllowMethod(_request.getMethod()) == false) {
		_abortRequestHandling(405);
	}
    
    //build full target
    std::string targetWithoutLocation = _request.getTarget();
    _serverSettings.setQueryString(targetWithoutLocation); // extracts the query string from the target
    
	std::string	target(_serverSettings.getRoot());
    int lengthOfLocation = _serverSettings.getLocation().length();
    targetWithoutLocation.erase(targetWithoutLocation.begin(), 
                                    targetWithoutLocation.begin() + lengthOfLocation - 1);
	//target.append(_request.getTarget());
	target.append(targetWithoutLocation);
    _request.setTarget(target);
    
    
	// Processing based on header fields
	const HttpMessage::headers_dict&	headers = _request.getHeaders();
    // Method is POST, but no header indicates existence of a body
    bool    postWithNoBody = (_request.getMethod() == POST
		&& headers.find("Content-Length") == headers.end()
		&& headers.find("Transfer-Encoding") == headers.end());
	if (postWithNoBody) {
		_abortRequestHandling(411);
        return ;
	}
    // Set bodysize
	if (headers.find("Content-Length") != headers.end())
	{
		std::string	sizeStr = headers.at("Content-Length");
        if (!isStrNum(sizeStr)) {
            _abortRequestHandling(400);
            return ;
        }
		ssize_t bodySize = std::atoi(sizeStr.c_str());
		if (bodySize > static_cast<ssize_t>(_serverSettings.getClientBodySize())) {
			_abortRequestHandling(413);
            return ;
        }
		_request.setBodySize(bodySize);
	}
	_setIsDone(true);
}

void	RequestParser::_abortRequestHandling(code_t statusCode)
{
	_response.setStatusCode(statusCode);
	_setIsDone(true);
}

RequestParser::code_t  RequestParser::_fillInRequestLineInfo(std::string requestLine)
{
    //printString(requestLine);
    //Initial sanitizer
	code_t	sanitizerResult = _httpSanitizer(requestLine);
	if (sanitizerResult != 200)
		{return (sanitizerResult);}
	if (requestLine.compare(0, 2, "\r\n") == 0)
        requestLine.erase(0, 2);

	//Set Method
    _request.setMethod(_strToMethod(requestLine));
    requestLine.erase(0, requestLine.find(" ") + 1);
    if (_request.getMethod() == UNKNOWN){
            return (400);
    }
	
	//Set target
	std::size_t separatorPos = requestLine.find(' ');
	
    _request.setTarget(requestLine.substr(0, separatorPos));
    requestLine.erase(0, separatorPos + 1);

	//Make sure the version is correct
	std::string	version = "HTTP/1.1";
	if (requestLine.compare(version) != 0)
        {return (400);}
	
	//All is good
	return (200);
}

RequestParser::code_t	RequestParser::_parseHeaderField(std::string headerField)
{
	std::string	fieldName;
	std::string	fieldValue;
	
	code_t  sanitizerResult = _httpSanitizer(headerField);
    if (sanitizerResult != 200){
        return (sanitizerResult);
    }
	//Parse fieldName	
	const size_t  colonPos = headerField.find(':');
    if (colonPos == headerField.npos)
        {return (400);}
    fieldName = headerField.substr(0, colonPos);
    if (fieldName.find_first_not_of(TOKEN_CHARS) != fieldName.npos)
        {return (400);}
	
	//Parse fieldValue
	fieldValue = headerField.substr(colonPos + 1);
    std::string::iterator   it = fieldValue.begin();
    while (*it == ' ' || *it == '\t')
        it++;
    fieldValue.erase(fieldValue.begin(), it);
    std::string::reverse_iterator   rit = fieldValue.rbegin();
    while (*rit == ' ' || *rit == '\t')
        rit++;
    fieldValue.resize(fieldValue.length() - (rit - fieldValue.rbegin()));

	//Match the server and location as soon as possible
	if (fieldName.compare("Host") == 0)
	{
		std::size_t	colonPos = fieldValue.find(':');
		_serverSettings.setServer(std::string(fieldValue, 0, colonPos));
		_serverSettings.setLocation(_request.getTarget());
	}
	//Place the header in the request.
	_request.addHeader(fieldName, fieldValue);
    return (200);
}

RequestParser::code_t	RequestParser::_httpSanitizer(std::string line)
{
	size_t  crPos = line.find('\r');;
    do
    {
        if (crPos != line.npos && line[crPos + 1] != '\n')
            return (400);
        crPos++;
        crPos = line.find('\r', crPos);
    } while (crPos != line.npos);
	return (200);
}

Method  RequestParser::_strToMethod(std::string str)
{
    static const std::string implementedMethods[4] = {"GET", "POST", "DELETE", "HEAD"};
    for (int i = 0; i < 4; i++)
	{
		if (str.compare(0, implementedMethods[i].length() + 1, implementedMethods[i] + " ") == 0)
			return (static_cast<Method>(i));
	}
    static const std::string otherMethods[] = {
        "PUT", "CONNECT", "OPTIONS", "TRACE", "PATCH"
    };
    for (int i = 0; i < 5; i++)
	{
		if (str.compare(0, otherMethods[i].length() + 1, otherMethods[i] + " ") == 0)
			return (NOT_IMPLEMENTED);
	}
	return (UNKNOWN);
}
