cmake_minimum_required(VERSION 3.0.0)
project(todofinder)

add_executable(ftodo main.c Todo.c Globals.c Configuration.c)

# distribute source with the binary in order to follow license
# https://github.com/cxong/tinydir
add_custom_command(TARGET ftodo POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/tinydir.h $<TARGET_FILE_DIR:ftodo>/tinydir.h
)