cmake_minimum_required(VERSION 3.24)
project(Practica_2)

set(CMAKE_CXX_STANDARD 17)

add_executable(Practica_2 code/src/main.cpp code/lib/indexadorHash.cpp code/lib/stemmer.cpp code/lib/indexadorinformacion.cpp code/lib/tokenizador.cpp code/include/stemmer.h code/include/tokenizador.h code/include/indexadorHash.h  code/include/indexadorinformacion.h ../Practica_1/code/lib/tokenizador.cpp)
