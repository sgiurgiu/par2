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

#ifndef PAR2_PAR2_FACTORY_H
#define PAR2_PAR2_FACTORY_H

#include <map>
#include <memory>

namespace par2 {

  template <
    class AbstractPacket,
    typename IdentifierType,
    typename PacketCreator = std::function<AbstractPacket()>>
class par2_factory
{
public:
  bool register_type(const IdentifierType& id, PacketCreator creator)
  {
    return types.emplace(id,creator).second;
  }
  bool unregister_type(const IdentifierType& id)
  {
    return types.erase(id) == 1;
  }
  AbstractPacket create_packet(const IdentifierType& id)
  {
    auto it = types.find(id);
    if(it != types.end())
    {
      return  it->second();      
    }
    //FIXME: Use a FactoryErrorPolicy
    throw std::runtime_error("Unknown type");
  }
  
private:  
  typedef std::map<IdentifierType,PacketCreator> TypeMap;
  TypeMap types;
};

}

#endif // PAR2_PAR2_FACTORY_H
