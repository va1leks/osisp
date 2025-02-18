cmake_minimum_required(VERSION 3.10)
project(dirwalk)

# Режим отладки
set(CMAKE_CXX_FLAGS_DEBUG "-g -ggdb -std=c11 -pedantic -W -Wall -Wextra")

# Режим выпуска
set(CMAKE_CXX_FLAGS_RELEASE "-std=c11 -pedantic -W -Wall -Wextra -Werror")

# Проверка наличия valgrind (опционально)
find_program(VALGRIND NAMES valgrind)

# Добавление исполняемого файла
add_executable(dirwalk src/dirwalk.c)

# Установка флагов компиляции в зависимости от типа сборки
if(CMAKE_BUILD_TYPE MATCHES "Debug")
    set_target_properties(dirwalk PROPERTIES COMPILE_FLAGS "${CMAKE_CXX_FLAGS_DEBUG}")
elseif(CMAKE_BUILD_TYPE MATCHES "Release")
    set_target_properties(dirwalk PROPERTIES COMPILE_FLAGS "${CMAKE_CXX_FLAGS_RELEASE}")
endif()

# Добавление команды для запуска с valgrind (опционально)
if(VALGRIND)
    add_custom_target(valgrind
        COMMAND ${VALGRIND} --leak-check=full --show-leak-details --track-origins=yes ./dirwalk
        DEPENDS dirwalk
    )
endif()
