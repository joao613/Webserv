/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nsouza-o <nsouza-o@student.42porto.com     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/22 14:24:14 by nsouza-o          #+#    #+#             */
/*   Updated: 2025/03/17 17:32:16 by nsouza-o         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP

// #include "utils.hpp"
#include "Location.hpp"

class Server {
private:
	int _serverId;
	std::vector<int> _listen;
	std::string _serverName;
	std::string _root;
	size_t _clientBodySize;
	std::vector<std::string> _index;
	std::map<int, std::string> _errorPage;
	std::map<std::string, std::string> _cgi;
	std::vector<std::string> _allowMethods;
	std::vector<Location> _locations;

public:
	Server();
	Server(const Server& src);
	Server& operator=(const Server& src);
	~Server();

	/* Setters */
	void setServerId(size_t serverId);
	void setListen(const std::vector<std::string>& port);
	void setServerName(const std::vector<std::string>& serverName);
	void setRoot(const std::vector<std::string>& root);
	void setClientBodySize(const std::vector<std::string>& clientLimit);
	void setIndex(const std::vector<std::string>& index);
	void setErrorPage(const std::vector<std::string>& errorPage);
	void setCgi(const std::vector<std::string>& cgi);
	void setLocation(std::vector<std::string>& serverVector, size_t i);
	void setAllowMethods(const std::vector<std::string>& allowMethods);
	Location fillLocation(std::vector<std::string>& serverVector, size_t begin, size_t end);

	/* Getters */
	int getListen(size_t portNb) const;
	int getListenSize(void) const;
	std::vector<int> getListenVec();
	std::string getServerName(void) const;
	std::string getRoot(void) const;
	size_t getClientBodySize(void) const;
	size_t getIndexSize() const;
	std::vector<std::string> getAllowMethods();
	std::string getIndex(size_t indexNbr) const;
	const std::map<int, std::string>& getErrorPage() const;
	const std::vector<Location>& getLocation() const;
	std::map<std::string, std::string> getCgi() const;
	bool isCgiExtension(std::string);
	std::string cgiExtensionHasASpecifcScript(std::string extension);


	//const Location& getOneLocation(std::string target) const;

	void setElements(std::string element);
};

typedef void (Server::*SetFunct)();
size_t getElementNbr(std::string element);

#endif // SERVER_HPP