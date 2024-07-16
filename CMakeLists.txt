cmake_minimum_required(VERSION 3.28)

project(swormEvolution)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/algoritms/include)

set(SERVER_FILES
	algoritms/src/Serverfunc.cpp
 	algoritms/src/DataBase.cpp
	algoritms/src/Logger.cpp
)

set(CLIENT_FILES
	algoritms/src/Clientfunc.cpp
	algoritms/src/Logger.cpp
)

set(MAIN_FILES
	algoritms/src/main.cpp
	algoritms/src/Evolution.cpp
	algoritms/src/Logger.cpp
)


#add_compile_options(-O3 -Wall -Wextra)
		
add_executable(server ${SERVER_FILES})
add_executable(client ${CLIENT_FILES})
add_executable(main ${MAIN_FILES})


