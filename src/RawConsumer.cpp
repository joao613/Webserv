/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RawConsumer.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ndo-vale <ndo-vale@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/06 10:33:10 by ndo-vale          #+#    #+#             */
/*   Updated: 2025/03/17 16:41:45 by ndo-vale         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "RawConsumer.hpp"
#include <ctime>

RawConsumer::RawConsumer(HttpResponse&  response, bool shouldPerformPost, /*std::string saveFileName,*/ size_t bodySize)
    : ABodyConsumer(response, shouldPerformPost), _bodySize(bodySize)
{}
RawConsumer::~RawConsumer(){}

size_t  RawConsumer::consume(data_t& data)
{
    size_t  bytesAm = std::min(data.size(), _bodySize);
    if (_shouldPerformPost) {
        _saveFile.write(reinterpret_cast<char*>(data.data()), bytesAm);
    }
    _bodySize -= bytesAm;
    if (_bodySize == 0) {
        if (_bodySize == 0 && data.size() > bytesAm) {
            _response.setStatusCode(400);
        }
        _setIsDone(true);
    }
    return (bytesAm);
}
