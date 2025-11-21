/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpResponse.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nsouza-o <nsouza-o@student.42porto.com     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/15 12:16:31 by ndo-vale          #+#    #+#             */
/*   Updated: 2025/03/14 14:55:34 by nsouza-o         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTP_RESPONSE_HPP
# define HTTP_RESPONSE_HPP

# define DEFAULT_PAGE_PATH "./.default/default.html"

# include "HttpMessage.hpp"
# include "ServerSettings.hpp"
# include "utils.hpp"

# include <iostream>

class HttpResponse: public HttpMessage
{
public:
	typedef int code_t;
	std::string cgiFile;
	std::streampos cgiBodyStarted;

	HttpResponse(ServerSettings& serverSettings);
	~HttpResponse();

	void		setStatusCode(code_t statusCode);
	code_t		getStatusCode(void) const;
	void		setBodyPath(std::string bodyPath);
	std::string	getBodyPath(void) const;
	//FOR DEBUG:
	void	printMessage(void);

private:
	typedef	std::map<std::string, std::string> header_map;
	static const header_map	_createFileTypeMap(void);
	static const header_map	_fileTypeMap;

	ServerSettings&	_serverSettings;
	unsigned int	_statusCode;
	std::string		_bodyPath;
	

	
};

#endif
