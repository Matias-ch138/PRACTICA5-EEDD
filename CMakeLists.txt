cmake_minimum_required(VERSION 3.10.0)
project(Practica-5 VERSION 0.1.0 LANGUAGES C CXX)

add_executable(prueba1 Arbol.cpp ThashArbol.cpp main1.cpp LectorCSV.cpp Especie.cpp Parque.cpp UTM.cpp)
add_executable(prueba2 Arbol.cpp ThashArbol.cpp main.cpp LectorCSV.cpp Especie.cpp Parque.cpp UTM.cpp CajaNido.cpp ControlParques.cpp)