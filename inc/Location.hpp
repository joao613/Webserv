/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nsouza-o <nsouza-o@student.42porto.com     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/10 19:16:04 by nsouza-o          #+#    #+#             */
/*   Updated: 2025/03/17 17:24:01 by nsouza-o         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LOCATION_HPP
#define LOCATION_HPP

#include "utils.hpp"

class Location {
private:
	std::string _specificPath;
	std::vector<std::string> _allowMethods;
	bool _autoindex;
	std::vector<std::string> _index;
	std::vector<std::string> _return;
	std::string _root;
	size_t _clientBodySize;
	std::map<std::string, std::string> _cgi;

public:
	Location();
	Location(const Location& src);
	Location& operator=(const Location& src);
	~Location();

	/* Setters */
	void setSpecificPath(std::string& specificPath);
	void setAllowMethods(std::vector<std::string>& allowMethods);
	void setAutoIndex(std::vector<std::string>& autoIndex);
	void setIndex(std::vector<std::string>& index);
	void setReturn(std::vector<std::string>& Return);
	void setRoot(std::vector<std::string>& root);
	void setClientBodySize(std::vector<std::string>& clientLimit);
	void setCgi(std::vector<std::string>& cgi);


	/* Getters */
	std::string getSpecificPath() const;
	bool getAllowMethods(std::string method) const;
	bool getAutoIndex() const;
	size_t getIndexSize() const;
	std::string getIndex(size_t indexNbr) const;
	std::vector<std::string> getReturn() const;
	std::vector<std::string> getAllowMethodsVec() const;
	std::string getRoot() const;
	std::map<std::string, std::string> getCgi() const;
	size_t getClientBodySize() const;
	bool isCgiExtension(std::string);
	std::string cgiExtensionHasASpecifcScript(std::string extension);
	
	void setLocationElements(std::string& element);
};

size_t getLocationNbr(std::string element);

#endif // LOCATION_HPP