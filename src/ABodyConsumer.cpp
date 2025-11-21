/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ABodyConsumer.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nsouza-o <nsouza-o@student.42porto.com     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 09:53:18 by ndo-vale          #+#    #+#             */
/*   Updated: 2025/03/17 14:15:17 by nsouza-o         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ABodyConsumer.hpp"

ABodyConsumer::ABodyConsumer(HttpResponse& response, bool shouldPerformPost)
    :_response(response), _shouldPerformPost(shouldPerformPost), _isDone(false)
{}
ABodyConsumer::~ABodyConsumer(){}

void    ABodyConsumer::setup(std::string saveFileName)
{
    if (_shouldPerformPost) {
        if (isDirectory(saveFileName)) {
            _response.setStatusCode(405);
            return ;
        }
        int i = 0;
        while (isFile(saveFileName)) {
		size_t	lastSlashPos = saveFileName.find_last_of('/');
		size_t	lastDotPos = saveFileName.find_last_of('.');
		size_t	insertionPos = (lastSlashPos > lastDotPos) ? saveFileName.length() : lastDotPos;
            saveFileName.insert(insertionPos, "(1)");
            i++;
            if (i >= 100) {
                _response.setStatusCode(409);
                return ;
            }
        }
        _saveFile.open(saveFileName.c_str(), std::ios::binary);
        if (_saveFile.fail()) {
		std::cerr << "lol " << saveFileName << std::endl;
            _response.setStatusCode(500);
            return ;
        }
    }
    _response.cgiFile = saveFileName;
}
bool    ABodyConsumer::isDone() {return (_getIsDone());}

void    ABodyConsumer::_setShouldPerformPost(bool newValue){_shouldPerformPost = newValue;}
bool    ABodyConsumer::_getShouldPerformPost(void) const {return (_shouldPerformPost);}
void    ABodyConsumer::_setIsDone(bool newValue){_isDone = newValue;}
bool    ABodyConsumer::_getIsDone(void) const {return (_isDone);}
