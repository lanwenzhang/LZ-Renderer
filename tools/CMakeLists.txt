#add cpp to FW variable
file(GLOB_RECURSE TOOLS ./ *.cpp)

#Compile FW to lib
add_library(tools ${TOOLS})