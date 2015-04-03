#include "par2.h"
#include "types.h"


#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

#ifndef NDEBUG
#include <stdio.h>
#endif


static enum Par2Result
check_magic(struct par2_packet_header* header) 
{
  if(memcmp(header->magic, magic, sizeof(magic)) != 0)
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
 
enum Par2Result
verify (const char *par_file)
{
  int file;
  enum Par2Result result = PAR2_OK;
  if((file = open(par_file, O_RDONLY)) == -1 )
  {
    return PAR2_ERROR_OPENFILE;
  }
  struct par2_packet_header header;
  if((result = check_header(file,&header)) != PAR2_OK)
  {
    goto exit;
  }
  
  if(memcmp(header.type, main_type, sizeof(main_type)) == 0)
  {
    
  }
  
#ifndef NDEBUG
  printf("type is %s, is main %d\n", header.type,(memcmp(header.type, main_type, sizeof(main_type)) == 0));
#endif
  
exit:  
  close(file);
  return result;
}
