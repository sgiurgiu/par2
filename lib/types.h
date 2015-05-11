#ifndef PAR2_TYPES_H
#define PAR2_TYPES_H

#include <cstdint>
#include <vector>
#include <string>
#include <array>
#include <fstream>

namespace par2 {
  
typedef std::basic_string<uint8_t>	ustring;
typedef std::vector<uint8_t>	byte_data;
typedef std::array<uint8_t,8> 		byte_array_8;
typedef std::array<uint8_t,16> 		byte_array_16;

static constexpr byte_array_8 magic = 			{'P', 'A', 'R', '2', '\0', 'P', 'K', 'T'};
static constexpr byte_array_16 main_type = 			{'P','A','R',' ','2','.','0','\0','M','a','i','n','\0','\0','\0','\0'};
static constexpr byte_array_16 filedesc_type = 		{'P','A','R',' ','2','.','0','\0','F','i','l','e','D','e','s','c'};
static constexpr byte_array_16 input_slice_checksum_type = 	{'P','A','R',' ','2','.','0','\0','I','F','S','C','\0','\0','\0','\0'};
static constexpr byte_array_16 recovery_slice_type = 	{'P','A','R',' ','2','.','0','\0','R','e','c','v','S','l','i','c'};
static constexpr byte_array_16 creator_type = 		{'P','A','R',' ','2','.','0','\0','C','r','e','a','t','o','r','\0'};
static constexpr byte_array_16 unicode_filename_type = 	{'P','A','R',' ','2','.','0','\0','U','n','i','F','i','l','e','N'};
static constexpr byte_array_16 ascii_comment_type = 	{'P','A','R',' ','2','.','0','\0','C','o','m','m','A','S','C','I'};
static constexpr byte_array_16 unicode_comment_type =       {'P','A','R',' ','2','.','0','\0','C','o','m','m','U','n','i'};
static constexpr byte_array_16 input_file_slice_type  =     {'P','A','R',' ','2','.','0','\0','F','i','l','e','S','l','i','c'};
static constexpr byte_array_16 recovery_slice_checksum_type = {'P','A','R',' ','2','.','0','\0','R','F','S','C','\0','\0','\0','\0'};
static constexpr byte_array_16 packed_main_type =           {'P','A','R',' ','2','.','0','\0','P','k','d','M','a','i','n','\0'};
static constexpr byte_array_16 packed_recovery_slice_type = {'P','A','R',' ','2','.','0','\0','P','k','d','R','e','c','v','S'};


struct par2_packet_header
{
    byte_array_8 magic = {};
    uint64_t length = {0};
    byte_array_16 packet_hash = {};
    byte_array_16 recovery_set_id = {};
    byte_array_16 type = {};
    void read(std::ifstream& in) 
    {
      in.read((char*)magic.data(),sizeof(uint8_t) * magic.size());
      in.read((char*)&length,sizeof(uint64_t));
      in.read((char*)packet_hash.data(),sizeof(uint8_t) * packet_hash.size());
      in.read((char*)recovery_set_id.data(),sizeof(uint8_t) * recovery_set_id.size());
      in.read((char*)type.data(),sizeof(uint8_t) * type.size());
    }
    
};

struct par2_packet
{
  par2_packet_header header = {};
  virtual void read(std::ifstream& in) = 0;
  
  virtual ~par2_packet() = default;
  par2_packet() = default;
  par2_packet(const par2_packet& ) = default;
  par2_packet& operator=(const par2_packet& ) = default;
  par2_packet(par2_packet && ) = default;
  par2_packet& operator=(par2_packet && ) = default;
};

struct par2_main : public par2_packet
{
  uint64_t slice_size = {0};
  uint32_t files_count = {0};
  std::vector<byte_array_16> recovery_files_ids = {};
  std::vector<byte_array_16> nonrecovery_files_ids = {};
  virtual void read(std::ifstream& in) override
  {
    in.read((char*)&slice_size,sizeof(uint64_t));
    in.read((char*)&files_count,sizeof(uint32_t));
    for(uint32_t i = 0; i< files_count; ++i)
    {
      byte_array_16 id;
      in.read((char*)id.data(),sizeof(uint8_t) * id.size());
      recovery_files_ids.push_back(id);
    }
    byte_array_16 tmp;
    uint32_t size_left = header.length - sizeof(header) - 12 - files_count * sizeof(uint8_t) * tmp.size();
    uint32_t non_recovery_files_count = size_left / (sizeof(uint8_t) * tmp.size());
    for(uint32_t i = 0; i< non_recovery_files_count; ++i)
    {
      byte_array_16 id;
      in.read((char*)id.data(),sizeof(uint8_t) * id.size());
      nonrecovery_files_ids.push_back(id);
    }
    
  }
};

struct par2_description : public par2_packet
{
    byte_array_16 file_id = {};
    byte_array_16 file_hash = {};
    byte_array_16 file_16k_hash = {};
    uint64_t length = {0};
    ustring name = {};
    virtual void read(std::ifstream& in) override
    {
      in.read((char*)file_id.data(),sizeof(uint8_t) * file_id.size());
      in.read((char*)file_hash.data(),sizeof(uint8_t) * file_hash.size());
      in.read((char*)file_16k_hash.data(),sizeof(uint8_t) * file_16k_hash.size());
      in.read((char*)&length,sizeof(uint64_t));
      uint64_t name_length = header.length - sizeof(header) - (3 * sizeof(uint8_t) * file_id.size()) - sizeof(uint64_t);
      name.resize(name_length);
      in.read((char*)name.data(), name_length * sizeof(ustring::value_type));
    }
};
struct hash_crc
{
    byte_array_16 hash = {};
    uint32_t crc = {0};
    void read(std::ifstream& in)
    {
      in.read((char*)hash.data(),sizeof(uint8_t) * hash.size());
      in.read((char*)&crc,sizeof(uint32_t));
    }
};
struct par2_input_slice_checksum : public par2_packet
{
  byte_array_16 file_id = {};
  std::vector<hash_crc> hash_crcs = {};
  virtual void read(std::ifstream& in) override
  {
    in.read((char*)file_id.data(),sizeof(uint8_t) * file_id.size());
    uint64_t hc_count = (header.length - sizeof(header) - (sizeof(uint8_t) * file_id.size())) / sizeof(hash_crc);
    for(uint64_t i = 0; i< hc_count; ++i)
    {
      hash_crc hc;
      hc.read(in);
      hash_crcs.push_back(hc);
    }
  }
};

struct par2_recovery_slice : public par2_packet
{
  uint32_t exponent = {0};
  byte_data data = {};
  virtual void read(std::ifstream& in) override
  {
    in.read((char*)&exponent,sizeof(uint32_t));
    uint64_t data_length = header.length - sizeof(header) - sizeof(uint32_t);
    data.resize(data_length);
    in.read((char*)data.data(),data_length * sizeof(byte_data::value_type));
  }
};

struct par2_creator : public par2_packet
{
  ustring creator_name = {};
  virtual void read(std::ifstream& in) override
  {
    uint64_t name_length = header.length - sizeof(header);
    creator_name.resize(name_length);
    in.read((char*)creator_name.data(),name_length * sizeof(ustring::value_type));
  }
};

struct par2_unicode_filename : public par2_packet
{
  byte_array_16 file_id = {};
  ustring name = {};
  virtual void read(std::ifstream& in) override
  {
    in.read((char*)file_id.data(),sizeof(uint8_t) * file_id.size());
    uint64_t name_length = header.length - sizeof(header) - (sizeof(uint8_t) * file_id.size());
    name.resize(name_length);
    in.read((char*)name.data(),name_length * sizeof(ustring::value_type));
  }
};
struct par2_ascii_comment : public par2_packet
{
  ustring comment = {};
  virtual void read(std::ifstream& in) override
  {
    uint64_t comment_length = header.length - sizeof(header);
    comment.resize(comment_length);
    in.read((char*)comment.data(),comment_length * sizeof(ustring::value_type));
  }
};
struct par2_unicode_comment : public par2_packet
{
  byte_array_16 file_id = {};
  ustring comment = {};
  virtual void read(std::ifstream& in) override
  {
    in.read((char*)file_id.data(),sizeof(uint8_t) * file_id.size());
    uint64_t comment_length = header.length - sizeof(header) - (sizeof(uint8_t) * file_id.size());
    comment.resize(comment_length);
    in.read((char*)comment.data(),comment_length * sizeof(ustring::value_type));
  }
};
struct par2_input_file_slice : public par2_packet
{
  byte_array_16 file_id = {};
  uint64_t index = {0};
  byte_data data = {};
  virtual void read(std::ifstream& in) override
  {
    in.read((char*)file_id.data(),sizeof(uint8_t) * file_id.size());
    in.read((char*)&index,sizeof(uint64_t));
    uint64_t data_length = header.length - sizeof(header) - (sizeof(uint8_t) * file_id.size()) - sizeof(uint64_t);
    data.resize(data_length);
    in.read((char*)data.data(),data_length * sizeof(byte_data::value_type));
  }
  
};
struct hash_crc_exponent
{
    byte_array_16 hash = {};
    uint32_t crc = {0};
    uint32_t exponent = {0};
    void read(std::ifstream& in)
    {
      in.read((char*)hash.data(),sizeof(uint8_t) * hash.size());
      in.read((char*)&crc,sizeof(uint32_t));
      in.read((char*)&exponent,sizeof(uint32_t));
    }
    
};
struct par2_recovery_file_slice_checksum : public par2_packet
{
  byte_array_16 file_id = {};
  uint64_t index = {0};
  std::vector<hash_crc_exponent> hash_crc_exponents = {};
  virtual void read(std::ifstream& in) override
  {
    in.read((char*)file_id.data(),sizeof(uint8_t) * file_id.size());
    in.read((char*)&index,sizeof(uint64_t));
    uint64_t hc_count = (header.length - sizeof(header) - (sizeof(uint8_t) * file_id.size()) - sizeof(uint64_t)) / sizeof(hash_crc_exponent);
    for(uint64_t i = 0; i< hc_count; ++i)
    {
      hash_crc_exponent hc;
      hc.read(in);
      hash_crc_exponents.push_back(hc);
    }
  }
};

struct par2_packed_main : public par2_packet
{
  uint64_t subslice_size = {0};
  uint64_t slice_size = {0};
  uint32_t file_count = {0};
  std::vector<byte_array_16> recovery_files_ids = {};
  std::vector<byte_array_16> nonrecovery_files_ids = {};
  virtual void read(std::ifstream& in)  override
  {
    in.read((char*)&subslice_size,sizeof(uint64_t));
    in.read((char*)&slice_size,sizeof(uint64_t));
    in.read((char*)&file_count,sizeof(uint32_t));
    for(uint32_t i = 0; i< file_count; ++i)
    {
      byte_array_16 id;
      in.read((char*)id.data(),sizeof(uint8_t) * id.size());
      recovery_files_ids.push_back(id);
    }
    byte_array_16 tmp;
    uint32_t size_left = header.length - sizeof(header) - 20 - file_count * sizeof(uint8_t) * tmp.size();
    uint32_t non_recovery_files_count = size_left / (sizeof(uint8_t) * tmp.size());
    for(uint32_t i = 0; i< non_recovery_files_count; ++i)
    {
      byte_array_16 id;
      in.read((char*)id.data(),sizeof(uint8_t) * id.size());
      nonrecovery_files_ids.push_back(id);
    }
    
  }
  
};
struct par2_packed_recovery : public par2_packet
{
  uint32_t exponent = {0};
  byte_data recovery_data = {};
  virtual void read(std::ifstream& in) override
  {    
    in.read((char*)&exponent,sizeof(uint32_t));
    uint64_t data_length = header.length - sizeof(header) - sizeof(uint32_t);
    recovery_data.resize(data_length);
    in.read((char*)recovery_data.data(),data_length * sizeof(byte_data::value_type));
  }
  
};

}//namespace par2
#endif