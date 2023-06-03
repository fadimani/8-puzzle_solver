cmake_minimum_required(VERSION 3.24)
project(miniProjet1)

set(CMAKE_CXX_STANDARD 17)

add_executable(miniProjet1 main.cpp  graphemat.h graphemat.cpp liregraphe.h liregraphe.cpp liste.cpp Astar_taquin.h Astar_taquin.cpp)
add_executable(Astar_puzzle graphemat.h graphemat.cpp liregraphe.h liregraphe.cpp liste.cpp )
