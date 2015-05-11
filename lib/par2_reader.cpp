/*
 * reads par information
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

#include "par2_reader.h"


#include <utility>
#include <fstream>

using namespace par2;

par2_reader::par2_reader(const std::string& par2_file):file(std::move(par2_file))
{
  factory.register_type(main_type, [](){return std::make_unique<par2_main>();});
  factory.register_type(filedesc_type, [](){return std::make_unique<par2_description>();});
  factory.register_type(input_slice_checksum_type , [](){return std::make_unique<par2_input_slice_checksum>();});
  factory.register_type(recovery_slice_type, [](){return std::make_unique<par2_recovery_slice>();});
  factory.register_type(creator_type, [](){return std::make_unique<par2_creator>();});
  factory.register_type(unicode_filename_type, [](){return std::make_unique<par2_unicode_filename>();});
  factory.register_type(ascii_comment_type, [](){return std::make_unique<par2_ascii_comment>();});
  factory.register_type(unicode_comment_type, [](){return std::make_unique<par2_unicode_comment>();});
  factory.register_type(input_file_slice_type, [](){return std::make_unique<par2_input_file_slice>();});
  factory.register_type(recovery_slice_checksum_type, [](){return std::make_unique<par2_recovery_file_slice_checksum>();});
  factory.register_type(packed_main_type, [](){return std::make_unique<par2_packed_main>();});
  factory.register_type(packed_recovery_slice_type, [](){return std::make_unique<par2_packed_recovery>();});
}

par2_reader::PacketsMap par2_reader::read_packets()
{
  PacketsMap packets;
  std::ifstream in(file, std::ios::binary);
  while(!in.eof())
  {
    par2_packet_header header;
    header.read(in);
    if(header.magic != magic)
    {      
      throw par2_reader_exception();
    }
    
    auto packet = factory.create_packet(header.type);
    packet->header = header;
    packet->read(in);
    packets.emplace(header.packet_hash,std::move(packet));
  }
  
  
  
  return std::move(packets);
}

