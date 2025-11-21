/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nsouza-o <nsouza-o@student.42porto.com     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/11 14:52:27 by nsouza-o          #+#    #+#             */
/*   Updated: 2025/03/20 16:59:45 by nsouza-o         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Location.hpp"

Location::Location() : _autoindex(false), _clientBodySize(0){}

Location::Location(const Location& src)
{
	*this = src;
}

Location& Location::operator=(const Location& src)
{
	if (this != &src)
	{
		_specificPath = src._specificPath;
		_clientBodySize = src._clientBodySize;
		_allowMethods = src._allowMethods;
		_autoindex = src._autoindex;
		_index = src._index;
		_return = src._return;
		_root = src._root;
		_cgi = src._cgi;
	}
	return (*this);
}

Location::~Location(){}

/* Setters */

void Location::setSpecificPath(std::string& specificPath)
{
	_specificPath = specificPath;
}

void Location::setAllowMethods(std::vector<std::string>& allowMethods)
{
	if (allowMethods.size() < 2){
			throw std::runtime_error("Invalid directive allow_methods in " + _specificPath + " location.");
	}
		
	std::vector<std::string> methods = allowMethods;
	
	for (size_t i = 1; i < methods.size(); i++)
	{
		if (methods[i] != "GET" && methods[i] != "POST" && methods[i] != "DELETE" && methods[i] != "HEAD")
			throw std::runtime_error("Invalid directive allow_methods in " + _specificPath + " location.");
	}
	methods.erase(methods.begin());
	_allowMethods = methods;
}

void Location::setAutoIndex(std::vector<std::string>& autoIndex)
{
	if (autoIndex.size() != 2)
		throw std::runtime_error("Autoindex directive must have only one value in location block " + _specificPath + ".");
	
	if (autoIndex[1] == "on")
	{
		_autoindex = true;
	}
	else if (autoIndex[1] == "off")
		_autoindex = false;
	else
		throw std::runtime_error("The value of autoindex directive must be on of off.");
}

void Location::setIndex(std::vector<std::string>& index)
{
	if (index.size() < 2)
		throw std::runtime_error("Host directive must have one value, at least.");
	
	for (size_t i = 1; i < index.size(); i++)
			_index.push_back(index[i]);	
}

void Location::setReturn(std::vector<std::string>& Return)
{
	if (Return.size() < 2)
		throw std::runtime_error("The Error Page directive must have a value.");
	if (Return.size() > 3)
		throw std::runtime_error("The Error Page directive must have a http code and a value.");
	
	std::string returnContent = Return[Return.size() - 1];
	
	if (Return.size() == 3)
	{
		for (size_t j = 0; j < Return[1].size(); j++)
		{
			if (!isdigit(Return[1][j]))
				throw std::runtime_error("Invalid Return directive: Expected a number value in http code.");
		}
		int code = atoi(Return[1].c_str());
		if (code < 100 || code > 600)
			throw std::runtime_error("Invalid HTTP code in Return directive.");
		_return.push_back(Return[1]);
		_return.push_back(returnContent);			
	}
	else
	{
		_return.push_back("200");
		_return.push_back(returnContent);
	}
}

void Location::setRoot(std::vector<std::string>& root)
{
	if (root.size() != 2)
		throw std::runtime_error("Root directive must not have more than one value");

	/* location must be a path from location  */
	
	std::string newRoot = "root/" + root[1];
	
	if (!isDirectory(newRoot))
			throw std::runtime_error("Root directive must have a directory path.");
	
	_root = newRoot;
}

void Location::setClientBodySize(std::vector<std::string>& clientLimit)
{
	if (clientLimit.size() != 2)
		throw std::runtime_error("Host directive must not have more than one value and can't be empty.");
	for (size_t j = 0; j < clientLimit[1].size(); j++)
		if (!isdigit(clientLimit[1][j]))
			throw std::runtime_error("Invalid Client Limit directive: Expected a number value grater than 0.");
	_clientBodySize = atoi(clientLimit[1].c_str());
}

void Location::setCgi(std::vector<std::string>& cgi)
{
	if (cgi.size() > 3)
		throw std::runtime_error("The Cgi directive must have only two parameters.");
	if (cgi[1].find('.') != 0)
		throw std::runtime_error("The Cgi directive must have a file extension in first parameter.");
	if (cgi.size() == 3){
		_cgi[cgi[1]] = cgi[2];
	} else {
		if (cgi[1].compare(".py")){
			throw std::runtime_error("The Webserv only accept a Python script.");
		}
		_cgi[cgi[1]] = "";
	}
}

/* Getters */
std::string Location::getSpecificPath() const
{
	return (_specificPath);
}

bool Location::getAllowMethods(std::string method) const
{
	for (size_t i = 0; i < _allowMethods.size(); i++)
	{
		if (!_allowMethods[i].compare(method))
			return (true);
	}
	return (false);
}

bool Location::getAutoIndex() const
{
	return (_autoindex);
}

size_t Location::getIndexSize() const
{
	return (_index.size());
}

std::string Location::getIndex(size_t indexNbr) const
{
	if (indexNbr > _index.size() -1)
		throw std::runtime_error("Index number out of range. Valid range: 0 to " + intToStr(_index.size() -1));
	
	return (_index[indexNbr]);
}

std::vector<std::string> Location::getAllowMethodsVec() const
{
	return (_allowMethods);
}

std::vector<std::string> Location::getReturn() const
{
	if (_return.size() == 0)
		return (std::vector<std::string>());
	return (_return);
}

std::string Location::getRoot() const
{
	return (_root);
}

size_t Location::getClientBodySize() const
{
	return (_clientBodySize);
}

std::map<std::string, std::string> Location::getCgi() const
{
	return (_cgi);
}


bool Location::isCgiExtension(std::string extension)
{
	std::map<std::string, std::string>::iterator it = _cgi.find(extension);
	
	if (it != _cgi.end()) {
		return (true);
	} else {
	    return (false);
	}
}

std::string Location::cgiExtensionHasASpecifcScript(std::string extension)
{
	std::map<std::string, std::string>::iterator it = _cgi.find(extension);
	if (it != _cgi.end()){
		return (it->second);
	} else {
		return ("");
	}
}

/* - */

void Location::setLocationElements(std::string& element)
{
	void (Location::*SetFunct[7])(std::vector<std::string>&) = {
		&Location::setAllowMethods,
		&Location::setAutoIndex,
		&Location::setIndex,
		&Location::setReturn, 
		&Location::setRoot,
		&Location::setClientBodySize,
		&Location::setCgi
		};
	
	std::vector<std::string> elementVector = splitServerStr(element);
	size_t nbr =  getLocationNbr(elementVector[0]);
	
	(this->*SetFunct[nbr])(elementVector);
}

size_t getLocationNbr(std::string element)
{
	std::string elementsKeys[7] = {
		"allow_methods",
		"autoindex", 
		"index", 
		"return", 
		"root",
		"client_body_size",
		"cgi"
		};
	
	for (size_t i = 0; i < 7; ++i) {
		if (element == elementsKeys[i])
			return (i);		
	}
	throw std::runtime_error("Invalid element on Location block: " + element);
}
