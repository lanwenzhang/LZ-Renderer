#add cpp to FW variable
file(GLOB_RECURSE RENDERER ./ *.cpp)

#Compile FW to lib
add_library(renderer ${RENDERER})
