/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpMessage.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nsouza-o <nsouza-o@student.42porto.com     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/16 11:10:38 by ndo-vale          #+#    #+#             */
/*   Updated: 2025/03/13 18:12:08 by nsouza-o         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HttpMessage.hpp"

HttpMessage::HttpMessage():_headersSize(0){}
HttpMessage::~HttpMessage(){}

void	HttpMessage::addHeader(std::string fieldName, std::string fieldValue)
{
	try {
		_headers.at(fieldName);
	} catch (std::out_of_range& e) {
		_headers[fieldName] = fieldValue;
	}
}
void	HttpMessage::addHeaderCgi(std::string fieldName, std::string fieldValue)
{
	_headers[fieldName] = fieldValue;
}
const HttpMessage::headers_dict&	HttpMessage::getHeaders(void) const
{
	return (_headers);
}

void	HttpMessage::setHeadersSize(size_t newValue) {_headersSize = newValue;}
size_t	HttpMessage::getHeadersSize(void) const {return (_headersSize);}