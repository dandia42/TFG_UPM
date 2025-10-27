# Establecer la versión mínima requerida de CMake
cmake_minimum_required(VERSION 3.20)

# Nombre del proyecto
project(TFG)

# Especificar el estándar de C++ que se usará
set(CMAKE_CXX_STANDARD 14)

# Opciones para deshabilitar warnings o evitar que se traten como errores
if (CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    add_compile_options(-Wno-error)  # Suprimir warnings como errores
endif()

# Especificar los directorios de inclusión
include_directories(${PROJECT_SOURCE_DIR}/include)
include_directories(${PROJECT_SOURCE_DIR}/lib)

# Incluir las librerías y directorios de OpenGL y GLUT
find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)

include_directories(${OPENGL_INCLUDE_DIR})
include_directories(${GLUT_INCLUDE_DIRS})

# Agregar todos los archivos fuente .cpp en el directorio src
file(GLOB_RECURSE SOURCES "${PROJECT_SOURCE_DIR}/src/*.cpp")

# Agregar todos los archivos .h de la carpeta lib como dependencias externas
file(GLOB_RECURSE LIB_HEADERS "${PROJECT_SOURCE_DIR}/lib/*.h")
file(GLOB_RECURSE LIBRARIES "${PROJECT_SOURCE_DIR}/lib/*.lib")

# Definir el ejecutable
add_executable(TFGExecutable ${SOURCES})

# Enlazar las bibliotecas de OpenGL y GLUT, junto con las externas
target_link_libraries(TFGExecutable ${OPENGL_LIBRARIES} ${GLUT_LIBRARIES} ${LIBRARIES})

# Especificar la salida binaria en el directorio build
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/build)
