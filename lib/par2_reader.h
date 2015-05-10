/*
 * <one line to give the program's name and a brief idea of what it does.>
 * Copyright (C) 2015  <copyright holder> <email>
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 * 
 */

#ifndef PAR2_READER_H
#define PAR2_READER_H

#include "par2_factory.h"
#include "types.h"

#include <map>
#include <string>

namespace par2 {
class par2_reader
{
public:
  par2_reader(const std::string& par2_file);
  typedef std::map<byte_array_16,std::unique_ptr<par2_packet>> PacketsMap;
  PacketsMap read_packets();
private:
  std::string file;
  par2_factory<std::unique_ptr<par2_packet>,byte_array_16> factory;  
};
}//namespace par2
#endif // PAR2_READER_H
