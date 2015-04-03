#ifndef PAR2_TYPES_H
#define PAR2_TYPES_H

typedef unsigned char uint8;
typedef unsigned short uint16;
typedef unsigned int uint32;
typedef unsigned long uint64;

static const uint8 magic[] = {'P', 'A', 'R', '2', '\0', 'P', 'K', 'T'};
static const uint8 main_type[] = 			{'P','A','R',' ','2','.','0','\0','M','a','i','n','\0','\0','\0','\0'};
static const uint8 filedesc_type[] = 			{'P','A','R',' ','2','.','0','\0','F','i','l','e','D','e','s','c'};
static const uint8 input_slice_checksum_type[] = 	{'P','A','R',' ','2','.','0','\0','I','F','S','C','\0','\0','\0','\0'};
static const uint8 recovery_slice_type[] = 		{'P','A','R',' ','2','.','0','\0','R','e','c','v','S','l','i','c'};
static const uint8 creator_type[] = 			{'P','A','R',' ','2','.','0','\0','C','r','e','a','t','o','r','\0'};
static const uint8 unicode_filename_type[] = 		{'P','A','R',' ','2','.','0','\0','U','n','i','F','i','l','e','N'};
static const uint8 ascii_comment_type[] = 		{'P','A','R',' ','2','.','0','\0','C','o','m','m','A','S','C','I'};

struct par2_packet_header
{
    uint8 magic[8];
    uint64 length;
    uint8 packet_hash[16];
    uint8 recovery_set_id[16];
    uint8 type[16];    
};

struct par2_main
{
  uint64 slice_size;
  uint32 files_count;
  uint8* recovery_files_ids[16];
  uint8* nonrecovery_files_ids[16];  
};

struct par2_description
{
    uint8 file_id[16];
    uint8 file_hash[16];
    uint8 file_16k_hash[16];
    uint64 length;
    uint8* name;
};
struct hash_crc
{
    uint8 hash[16];
    uint32 crc;
};
struct par2_input_slice_checksum
{
  uint8 file_id[16];
  struct hash_crc* hash_crc_array;  
};

struct par2_recovery_slice
{
  uint32 exponent;
  uint8* data;  
};

struct par2_creator
{
  uint8* creator_name;  
};

struct par2_unicode_filename
{
  uint8 file_id[16];
  uint8* name;
};
struct par2_ascii_comment
{
  uint8* comment;
};
struct par2_unicode_comment
{
  uint8 file_id[16];
  uint8* comment;  
};
struct par2_input_file_slice
{
  uint8 file_id[16];
  uint64 index;  
  uint8* data;  
};
struct hash_crc_exponent
{
    uint8 hash[16];
    uint32 crc;
    uint32 exponent;
};
struct par2_recovery_file_slice_checksum
{
  uint8 file_id[16];
  uint64 index;  
  struct hash_crc_exponent* hash_crc_exponent_array; 
};

struct par2_packed_main
{
  uint64 subslice_size;
  uint64 slice_size;
  uint32 file_count;
  uint8* recovery_files_ids[16];
  uint8* nonrecovery_files_ids[16];    
};
struct par2_packed_recovery
{
  uint32 exponent;
  uint8* recovery_data;
};


#endif