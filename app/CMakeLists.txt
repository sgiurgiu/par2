include_directories(${CMAKE_SOURCE_DIR}/app/src ${CMAKE_SOURCE_DIR}/lib)



add_executable(par2 main.c)


target_link_libraries(par2 par2_lib)

install(TARGETS par2 RUNTIME DESTINATION bin)
