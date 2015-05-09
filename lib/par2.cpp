#include "par2.h"
#include "types.h"
#include "par2_packet.h"

#include<vector>

enum Par2Result
verify (const char *par_file)
{
    std::vector<int> aa;
  enum Par2Result result = read_mainfile_packets(par_file);
  
  return result;
}
