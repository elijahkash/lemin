# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    .CMakeLists.txt                                    :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: mtrisha <mtrisha@student.21-school.ru>     +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created:                     by mtrisha           #+#    #+#              #
#    Updated:                     by mtrisha          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

cmake_minimum_required(VERSION 3.15.3)

project(lemin)

set(BUILD_FLAGS "-O3 -Wall -Wextra -Werror")
set(DEBUG_FLAGS "-g3 -Wall -Wextra")
set(DST_NAME lem-in)
set(DEBUG_NAME lem-in.debug)
set(SRC_DIR ${PROJECT_SOURCE_DIR}/sources/)
set(INC_DIR ${PROJECT_SOURCE_DIR}/includes/)
set(LIBFT_DIR ${PROJECT_SOURCE_DIR}/libft/)

MACRO(INCLUDE_DIRS return_list)
	FILE(GLOB_RECURSE new_list ${INC_DIR}*.h)
	SET(dir_list "")
	FOREACH(file_path ${new_list})
		GET_FILENAME_COMPONENT(dir_path ${file_path} PATH)
		SET(dir_list ${dir_list} ${dir_path})
	ENDFOREACH()
	LIST(REMOVE_DUPLICATES dir_list)
	SET(${return_list} ${dir_list})
ENDMACRO()
set(INC_DIRS ./)
INCLUDE_DIRS(INC_DIRS)
FILE(GLOB_RECURSE LEMIN_SOURCES ${SRC_DIR}*.c)

add_subdirectory(${LIBFT_DIR})

add_executable(${DST_NAME} ${LEMIN_SOURCES})
target_include_directories(${DST_NAME} PUBLIC ${LIBFT_DIR}includes/)
target_include_directories(${DST_NAME} PUBLIC ${INC_DIRS})
target_link_directories(${DST_NAME} PUBLIC ${LIBFT_DIR})
target_link_libraries(${DST_NAME} libft)
set_target_properties(${DST_NAME} PROPERTIES
		RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}
		OUTPUT_NAME ${DST_NAME}
		COMPILE_FLAGS ${BUILD_FLAGS})

add_dependencies(${DST_NAME} libft)

add_executable(debug EXCLUDE_FROM_ALL ${LEMIN_SOURCES})
target_include_directories(debug PUBLIC ${LIBFT_DIR}includes/)
target_include_directories(debug PUBLIC ${INC_DIRS})
target_link_directories(debug PUBLIC ${LIBFT_DIR})
target_link_libraries(debug libftdebug)
set_target_properties(debug PROPERTIES
		RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}
		OUTPUT_NAME ${DEBUG_NAME}
		COMPILE_FLAGS ${DEBUG_FLAGS}
		COMPILE_DEFINITIONS DEBUG)

add_dependencies(debug libftdebug)
