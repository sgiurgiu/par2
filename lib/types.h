#ifndef PAR2_TYPES_H
#define PAR2_TYPES_H

#include <cstdint>
#include <vector>
#include <string>
#include <array>

typedef std::basic_string<uint8_t>	ustring;
typedef std::vector<uint8_t>	byte_data;
typedef std::array<uint8_t,8> 		byte_array_8;
typedef std::array<uint8_t,16> 		byte_array_16;

static const byte_array_8 magic = 			{'P', 'A', 'R', '2', '\0', 'P', 'K', 'T'};
static const byte_array_16 main_type = 			{'P','A','R',' ','2','.','0','\0','M','a','i','n','\0','\0','\0','\0'};
static const byte_array_16 filedesc_type = 		{'P','A','R',' ','2','.','0','\0','F','i','l','e','D','e','s','c'};
static const byte_array_16 input_slice_checksum_type = 	{'P','A','R',' ','2','.','0','\0','I','F','S','C','\0','\0','\0','\0'};
static const byte_array_16 recovery_slice_type = 	{'P','A','R',' ','2','.','0','\0','R','e','c','v','S','l','i','c'};
static const byte_array_16 creator_type = 		{'P','A','R',' ','2','.','0','\0','C','r','e','a','t','o','r','\0'};
static const byte_array_16 unicode_filename_type = 	{'P','A','R',' ','2','.','0','\0','U','n','i','F','i','l','e','N'};
static const byte_array_16 ascii_comment_type = 	{'P','A','R',' ','2','.','0','\0','C','o','m','m','A','S','C','I'};



struct par2_packet_header
{
    byte_array_8 magic;
    uint64_t length;
    byte_array_16 packet_hash;
    byte_array_16 recovery_set_id;
    byte_array_16 type;
};

template<typename T> class par2_packet
{
  
private:
  par2_packet_header header;
  T body;
};

struct par2_main
{
  uint64_t slice_size;
  uint32_t files_count;
  std::vector<byte_array_16> recovery_files_ids;
  std::vector<byte_array_16> nonrecovery_files_ids;
};

struct par2_description
{
    byte_array_16 file_id;
    byte_array_16 file_hash;
    byte_array_16 file_16k_hash;
    uint64_t length;
    ustring name;
};
struct hash_crc
{
    byte_array_16 hash;
    uint32_t crc;
};
struct par2_input_slice_checksum
{
  byte_array_16 file_id;
  std::vector<hash_crc> hash_crcs;
};

struct par2_recovery_slice
{
  uint32_t exponent;
  byte_data data;
};

struct par2_creator
{
  ustring creator_name;
};

struct par2_unicode_filename
{
  byte_array_16 file_id;
  ustring name;
};
struct par2_ascii_comment
{
  ustring comment;
};
struct par2_unicode_comment
{
  byte_array_16 file_id;
  ustring comment;
};
struct par2_input_file_slice
{
  byte_array_16 file_id;
  uint64_t index;
  byte_data data;
};
struct hash_crc_exponent
{
    byte_array_16 hash;
    uint32_t crc;
    uint32_t exponent;
};
struct par2_recovery_file_slice_checksum
{
  byte_array_16 file_id;
  uint64_t index;  
  std::vector<hash_crc_exponent> hash_crc_exponents; 
};

struct par2_packed_main
{
  uint64_t subslice_size;
  uint64_t slice_size;
  uint32_t file_count;
  std::vector<byte_array_16> recovery_files_ids;
  std::vector<byte_array_16> nonrecovery_files_ids;
};
struct par2_packed_recovery
{
  uint32_t exponent;
  byte_data recovery_data;
};


#endif