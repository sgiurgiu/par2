#include "par2.h"
#include "types.h"
#include "par2_reader.h"

#include<vector>

using namespace par2;

enum Par2Result
verify (const char *par_file)
{

  Par2Result result = PAR2_OK;// = read_mainfile_packets(par_file);
  par2_reader reader(par_file);
  par2_reader::PacketsMap packets = reader.read_packets();
  
  
  return result;
}
