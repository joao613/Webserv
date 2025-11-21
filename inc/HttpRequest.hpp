/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpRequest.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ndo-vale <ndo-vale@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/15 11:57:32 by ndo-vale          #+#    #+#             */
/*   Updated: 2025/02/16 21:45:24 by ndo-vale         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTP_REQUEST_HPP
# define HTTP_REQUEST_HPP

# include "HttpMessage.hpp"
# include "utils.hpp"

# include <iostream>

class HttpRequest: public HttpMessage
{
public:	
	HttpRequest();
	~HttpRequest();

	void		setMethod(Method method);
	Method		getMethod(void) const;
	void		setTarget(std::string target);
	std::string	getTarget(void) const;
	void		setBodySize(size_t bodySize);
	ssize_t		getBodySize(void) const;

	//FOR DEBUG:
	void	printMessage(void);
private:
	Method		_method;
	std::string	_target;
	ssize_t		_bodySize;
};

#endif
