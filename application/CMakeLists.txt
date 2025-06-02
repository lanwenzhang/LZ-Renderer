#add cpp to WRAPPER variable
file(GLOB_RECURSE APP ./ *.cpp *.cc *.c)

#Compile WRAPPER to lib
add_library(app ${APP})