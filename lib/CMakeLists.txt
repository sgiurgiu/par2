
set (PAR2_LIB_SRC par2.cpp par2_reader.cpp)



add_library(par2_lib SHARED ${PAR2_LIB_SRC})

#target_link_libraries(par2_lib LIB_UV)

#install(TARGETS par2 RUNTIME DESTINATION bin)
