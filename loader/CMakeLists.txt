#add cpp to WRAPPER variable
file(GLOB_RECURSE LOADER ./ *.cpp *.cc *.c)

#Compile WRAPPER to lib
add_library(loader ${LOADER})