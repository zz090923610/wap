cmake_minimum_required(VERSION 2.8)
project( mapTest )
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")
find_package( OpenCV REQUIRED )
find_package( gsl REQUIRED)


add_executable( mapTest mapTest.cpp Step.h Step.cpp ImageSet.cpp ImageSet.h wallElement.h wallElement.cpp sensorData.h sensorData.cpp HDEController.h HDEController.cpp accessPoint.h accessPoint.cpp Trilateration.h Trilateration.cpp particle.h particle.cpp particlePosition.h particlePosition.cpp Constants.h pixel.h pixel.cpp apStatus.h apStatus.cpp point.h point.cpp room.h room.cpp wifiVector.h wifiVector.cpp ProbTableElement.h ProbTableElement.cpp Constants.cpp)


target_link_libraries( mapTest  gsl)
target_link_libraries( mapTest  gslcblas)
target_link_libraries( mapTest ${OpenCV_LIBS})

link_directories(/usr/lib /usr/local/lib)
