/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpRequest.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ndo-vale <ndo-vale@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/16 11:16:26 by ndo-vale          #+#    #+#             */
/*   Updated: 2025/03/12 12:45:15 by ndo-vale         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HttpRequest.hpp"

HttpRequest::HttpRequest(): _method(UNKNOWN), _target(""), _bodySize(-1){}
HttpRequest::~HttpRequest(){}

void		HttpRequest::setMethod(Method method){_method = method;}
Method		HttpRequest::getMethod(void) const {return (_method);}
void		HttpRequest::setTarget(std::string target) {_target = target;}
std::string	HttpRequest::getTarget(void) const {return (_target);}
void		HttpRequest::setBodySize(size_t bodySize) {_bodySize = bodySize;}
ssize_t		HttpRequest::getBodySize(void) const {return (_bodySize);}

void	HttpRequest::printMessage(void)
{
	const std::string methods[7] = {"GET", "POST", "DELETE", "HEAD", "LOL U REKT", "NOT IMPLEMENTED", "UNKNOWN"};
	std::cerr << "----REQUEST----" << std::endl;
	std::cerr << "Method: " << methods[_method] << std::endl;
	
	std::cerr << "Target: " << _target << std::endl;
	
	std::cerr << "Headers:" << std::endl;
	for (headers_dict::iterator it = _headers.begin(); it != _headers.end(); it++)
	{
		std::cerr << it->first << ": " << it->second << std::endl;
	}
}
