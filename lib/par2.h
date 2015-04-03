#ifndef PAR2_H
#define PAR2_H

#ifdef __cplusplus
extern "C" {
#endif

enum Par2Result {
  PAR2_ERROR_NOTAPAR2FILE = -100,
  PAR2_ERROR_OPENFILE = -99,
  PAR2_OK = 0,
  PAR2_REPAIR_NEEDED = 1,
  PAR2_CANNOT_REPAIR = 2
};  

enum Par2Result verify(const char* par_file);



#ifdef __cplusplus
}
#endif

#endif