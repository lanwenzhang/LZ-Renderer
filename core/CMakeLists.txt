#add cpp to WRAPPER variable
file(GLOB_RECURSE CORE ./ *.cpp)

#Compile WRAPPER to lib
add_library(core ${CORE})