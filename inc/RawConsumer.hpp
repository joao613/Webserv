/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RawConsumer.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ndo-vale <ndo-vale@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/06 10:25:55 by ndo-vale          #+#    #+#             */
/*   Updated: 2025/03/13 11:14:02 by ndo-vale         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RAW_CONSUMER_HPP
# define RAW_CONSUMER_HPP

# include "ABodyConsumer.hpp"
# include "StateMachine.hpp"
# include <cstdlib> //for strtol
# include <fstream>
# include "utils.hpp"

class RawConsumer: public ABodyConsumer
{
public:
    RawConsumer(HttpResponse&  response, bool shouldPerformPost, /*std::string saveFileName,*/ size_t bodySize);
    ~RawConsumer();

    size_t  consume(data_t& data);
private:
    size_t          _bodySize;
    //std::ofstream   _saveFile;
};

#endif