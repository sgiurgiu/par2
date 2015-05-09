#include "par2_packet.h"
#include "types.h"

#include <string.h>
#include <assert.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#ifndef NDEBUG
#include <iostream>
#endif

static enum Par2Result
check_magic(struct par2_packet_header* header) 
{
  if(header->magic != magic)
  {
    return PAR2_ERROR_NOTAPAR2FILE;
  }
  
  return PAR2_OK;
}

static enum Par2Result
check_header(int file, struct par2_packet_header* header) 
{
  enum Par2Result result = PAR2_OK;
  int read_amount = 0;
  if((read_amount = read(file,header, sizeof(*header))) != sizeof(*header))
  {
    return PAR2_ERROR_NOTAPAR2FILE;
  }
  if((result = check_magic(header)) != PAR2_OK)
  {
    return result;
  }
  assert((header->length - 64) % 4 == 0);//body length must be a multiple of 4
  return PAR2_OK;
}

enum Par2Result read_mainfile_packets(const char* file_name)
{
    int file;
  enum Par2Result result = PAR2_OK;
  if((file = open(file_name, O_RDONLY)) == -1 )
  {
    return PAR2_ERROR_OPENFILE;
  }
  struct par2_packet_header header;
  if((result = check_header(file,&header)) != PAR2_OK)
  {
    goto exit;
  }
  
#ifndef NDEBUG
  std::cout <<"type is "<<header.type.data()<<", is filedesc "<<(header.type == filedesc_type)<<"\n";
#endif
  
  
exit:
  close(file);
  return result;  
}