/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestPerformer.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ndo-vale <ndo-vale@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/27 20:09:54 by ndo-vale          #+#    #+#             */
/*   Updated: 2025/03/21 09:40:55 by ndo-vale         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "RequestPerformer.hpp"

RequestPerformer::RequestPerformer(HttpRequest& request, HttpResponse& response,
                                    ServerSettings& serverSettings)
    : AMessageHandler(request, response), _serverSettings(serverSettings),
        _consumeMode(false), _bodyConsumer(NULL)
{}
RequestPerformer::~RequestPerformer(){}

size_t    RequestPerformer::perform(const data_t& data)
{
    size_t  dataConsumed = 0;
    if (_getConsumeMode() == false) {
        switch (_request.getMethod()) {
            case GET:
                _performGet();
                _setConsumeMode(true);
                break;
            case DELETE:
                _performDelete();
                _setConsumeMode(true);
                break;
            case POST:
				_setConsumeMode(true); //Post performing is done during the body receiving
                break;
            default:
                throw (std::runtime_error("Request manager achieved an unexpected state."));
        };
    }
    if (_getConsumeMode() == true) {
		dataConsumed += _consumeBody(data);
    }
    return (dataConsumed);
}

void    RequestPerformer::activateConsumeMode(void)
{
    _setConsumeMode(true);
}

void    RequestPerformer::_setConsumeMode(bool newValue) {_consumeMode = newValue;}
bool    RequestPerformer::_getConsumeMode(void) const {return (_consumeMode);}

void    RequestPerformer::_performGet(void)
{
    std::string	target(_request.getTarget());
	if (isDirectory(target)) // If target is directory
	{		
		if (!_serverSettings.getAutoIndex())
		{
			target.append("/" + _serverSettings.getIndex());
			if (isFile(target)) {
				_response.setBodyPath(target);
			} else {
				_response.setStatusCode(404);
			}
			
		} else {
			_createAutoIndex(target);
			_response.setBodyPath(".default/autoindex.html");
		}
	}
	else if (isFile(target)) {
		_response.setBodyPath(target);
	} else {
		_response.setStatusCode(404);
	}
}

void    RequestPerformer::_performDelete(void)
{
	std::string	target(_request.getTarget());
    if (!isFile(target)) {
		_response.setStatusCode(404);
		return ;
	}
	if (access(target.c_str(), W_OK) != 0) {
		_response.setStatusCode(403);
		return ;
	}
	std::remove(target.c_str());
	_response.setStatusCode(204);
}

size_t  RequestPerformer::_consumeBody(data_t data)
{
	size_t	dataConsumed = 0;
	if (!_bodyConsumer) {
		bool shouldPerformPost = (_request.getMethod() == POST && _response.getStatusCode() == 200);
		const HttpMessage::headers_dict	requestHeaders = _request.getHeaders();
		if (requestHeaders.find("Transfer-Encoding") != requestHeaders.end()) {
			_bodyConsumer = new ChunkedConsumer(_response, shouldPerformPost);
		} else if (requestHeaders.find("Content-Length") != requestHeaders.end()) {
			_bodyConsumer = new RawConsumer(_response, shouldPerformPost, _request.getBodySize());
		} else {
			_setIsDone(true);
			return (dataConsumed);
		}
		if (_response.getStatusCode() == 200) {
			_bodyConsumer->setup(_request.getTarget());
			if (_response.getStatusCode() != 200) {
				_setIsDone(true);
				if (_bodyConsumer) {
					delete _bodyConsumer;
					_bodyConsumer = NULL;
				}
				return (dataConsumed);
			}
		}
	}
	dataConsumed += _bodyConsumer->consume(data);
	if (_bodyConsumer->isDone()) {
		delete _bodyConsumer;
		_bodyConsumer = NULL;
		_setIsDone(true);
	}
    return (dataConsumed);
}

void	RequestPerformer::_createAutoIndex(std::string target)
{	
	DIR*	dir;
	struct dirent*	dp;
	if ((dir = opendir(target.c_str())) == NULL) {
        throw std::runtime_error("Could not open directory.");
    }
	std::ofstream	autoindexFile;
	autoindexFile.open(".default/autoindex.html");
	autoindexFile << 
		"<html>"
			"<head>"
				"<title>Index of " << target << "</title>"
			"</head>"
			"<body>"
				"<h1>Index of " << target << "</h1>"
				"<hr>"
				"<ul>";
	while ((dp = readdir(dir)) != NULL)
	{
		bool isDir = isDirectory(target + "/" + dp->d_name);
		autoindexFile << "<li><a href=\"" << dp->d_name << (isDir ? "/" : "") <<"\">" << dp->d_name << "</a></li>";
	}
	autoindexFile <<
				"</ul>"
			"</body>"
		"</html>";
	closedir(dir);
	autoindexFile.close();
}
