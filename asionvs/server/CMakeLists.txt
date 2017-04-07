
project(server)  # change to name of this module

# maybe some other include directories are necessary
# just in that case: append those by separating it with a space
include_directories(include)

# maybe some data is needed for the executable...
#FILE(COPY data1.txt data2.txt DESTINATION ${CMAKE_CURRENT_BINARY_DIR})


# this can be used to output a message
#message(${CMAKE_SOURCE_DIR}/counter/include)

file(GLOB sources src/*.cpp include/*.h)

# change the name of your executable IF desired
#   default is the name of the whole app
#   replace "${appname}" with something appropriate
add_executable(server  ${sources})

# uncomment and change (add or remove) if necessary
#set(MY_LIB_LINK_LIBRARIES -L$ENV{HOME}/SFML-2.2/lib -lsfml-graphics -lsfml-window -lsfml-system -lsqlite3)

# change the name of the library appropriately
#target_link_libraries(server ${MY_LIB_LINK_LIBRARIES})


#set (SQLLINKLIBS -lsqlite3 -lrt -lpthread)
#target_link_libraries(server${SQLLINKLIBS})


set (PTHREADLIBS -lpthread -lboost_thread -lboost_system )
target_link_libraries(server ${PTHREADLIBS})

install(TARGETS server
  RUNTIME DESTINATION "bin")


