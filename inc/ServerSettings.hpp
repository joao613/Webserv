/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerSettings.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nsouza-o <nsouza-o@student.42porto.com     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/03 19:43:52 by nsouza-o          #+#    #+#             */
/*   Updated: 2025/03/17 17:26:02 by nsouza-o         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVERSETTINGS_HPP
#define SERVERSETTINGS_HPP

#include <iostream>
#include "ConfigFile.hpp"

class ServerSettings {
private:
	int _port;
	std::string							_queryString;
	
	std::string _serverName;
	std::string	_location;
	std::string _root;
	size_t _clientBodySize;
	std::string _index; /* The first file found will be served */
	std::map<int, std::string> _errorPages;
	std::map<std::string, std::string> _cgi;
	
	std::vector<Method> _allowMethods;
	bool _autoindex;
	int _returnCode;
	std::string _returnURL;
	
	ConfigFile& _src;

	void setIndex(Server& server);
	void setAllowMethods(std::vector<std::string> getAllowMethods);
	void setReturn(Location location);
	void setIndexLocation(Location location);
	
public:
	ServerSettings(ConfigFile& src, int port);
	ServerSettings(const ServerSettings& src);
	ServerSettings& operator=(const ServerSettings& src);
	~ServerSettings();

	void setServer(std::string serverName);
	void setLocation(std::string target);
	void setQueryString(std::string& target);


	const std::string& getServerName() const;
	const std::string getLocation() const;
	const std::string& getRoot() const;
	size_t getClientBodySize() const;
	const std::string& getIndex() const;
	const std::string getErrorPage(int errorCode) const;
	bool getAllowMethod(Method method) const;
	bool getAutoIndex() const;
	int getReturnCode() const;
	std::string getQueryString() const;
	const std::string& getReturnURL() const;
	bool isCgiExtension(std::string target);
	std::string cgiExtensionHasASpecifcScript(std::string extension);
	bool isCgi(std::string target);
};

#endif // SERVERSETTINGS_HPP
/*
SETTINGS:
-serverName: 			"default"
-root: 					"./root"
-clientBodySizeLimit:	1000*1000
-index:					"html/index.html"
-errorPages:				<error>: "./.default/<error>.html" (where error is the error code)
-allowMethods:			GET, POST, DELETE (the enum items)
-autoindex:				false
-returnCode:			-1
-returnURL:				""				
*/
