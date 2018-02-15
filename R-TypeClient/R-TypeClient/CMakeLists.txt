cmake_minimum_required(VERSION 3.5)

project(R-TypeClient)

include_directories(include SFML/include)

set (SRC 
src/Entity.cpp src/Component.cpp src/System.cpp src/Ressources.cpp src/R-TypeClient.cpp src/SystemGUI.cpp src/TextComponent.cpp src/PropertyComponent.cpp src/AnimationComponent.cpp src/Network.cpp src/SystemManager.cpp src/SystemController.cpp src/SelectionComponent.cpp src/CountComponent.cpp src/ScrollComponent.cpp src/NotificationComponent.cpp src/SoundManager.cpp src/SfmlTcpSocket.cpp src/SfmlUdpSocket.cpp
     include/Entity.h include/Component.h include/System.h include/Ressources.h include/SystemGUI.h include/TextComponent.h include/PropertyComponent.h include/AnimationComponent.h include/Network.h include/SystemManager.h include/Protocol.h include/SystemController.h include/SelectionComponent.h include/CountComponent.h include/ScrollComponent.h include/NotificationComponent.h include/SoundManager.h include/ITCPSocket.h include/IUDPSocket.h include/SfmlTcpSocket.h include/SfmlUdpSocket.h)

add_executable(R-TypeClient
	${SRC}
)


if (UNIX)
set(LIBRARIES
)
endif (UNIX)

if (WIN32)
set(LIBRARIES
# SFML/lib/sfml-audio
 #SFML/lib/sfml-graphics
 #SFML/lib/sfml-window
 #SFML/lib/sfml-main
 #SFML/lib/sfml-system
 #SFML/lib/sfml-network

  SFMLDebug/lib/sfml-audio-d
 SFMLDebug/lib/sfml-graphics-d
 SFMLDebug/lib/sfml-window-d
 SFMLDebug/lib/sfml-main-d
 SFMLDebug/lib/sfml-system-d
 SFMLDebug/lib/sfml-network-d
)
endif (WIN32)
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -fpermissive")

#add_library(sfml-audio SHARED IMPORTED)
# set_property(TARGET sfml-audio PROPERTY IMPORTED_LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/SFML/lib/sfml-audio-2.dll)
# set_property(TARGET sfml-audio PROPERTY IMPORTED_IMPLIB ${CMAKE_CURRENT_SOURCE_DIR}/SFML/lib/sfml-audio.lib)
 
#add_library(sfml-graphics SHARED IMPORTED)
# set_property(TARGET sfml-graphics PROPERTY IMPORTED_LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/SFML/lib/sfml-graphics-2.dll)
# set_property(TARGET sfml-graphics PROPERTY IMPORTED_IMPLIB ${CMAKE_CURRENT_SOURCE_DIR}/SFML/lib/sfml-graphics.lib)#

#add_library(sfml-window SHARED IMPORTED)
# set_property(TARGET sfml-window PROPERTY IMPORTED_LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/SFML/lib/sfml-window-2.dll)
# set_property(TARGET sfml-window PROPERTY IMPORTED_IMPLIB ${CMAKE_CURRENT_SOURCE_DIR}/SFML/lib/sfml-window.lib)

#add_library(sfml-main SHARED IMPORTED)
# set_property(TARGET sfml-main PROPERTY IMPORTED_LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/SFML/lib/sfml-main-2.dll)
# set_property(TARGET sfml-main PROPERTY IMPORTED_IMPLIB ${CMAKE_CURRENT_SOURCE_DIR}/SFML/lib/sfml-main.lib)

#add_library(sfml-system SHARED IMPORTED)
# set_property(TARGET sfml-system PROPERTY IMPORTED_LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/SFML/lib/sfml-system-2.dll)
# set_property(TARGET sfml-system PROPERTY IMPORTED_IMPLIB ${CMAKE_CURRENT_SOURCE_DIR}/SFML/lib/sfml-system.lib)

#add_library(sfml-network SHARED IMPORTED)
# set_property(TARGET sfml-network PROPERTY IMPORTED_LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/SFML/lib/sfml-network-2.dll)
# set_property(TARGET sfml-network PROPERTY IMPORTED_IMPLIB ${CMAKE_CURRENT_SOURCE_DIR}/SFML/lib/sfml-network.lib)




 add_library(sfml-audio-d SHARED IMPORTED)
 set_property(TARGET sfml-audio-d PROPERTY IMPORTED_LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/SFMLDebug/lib/sfml-audio-d-2.dll)
 set_property(TARGET sfml-audio-d PROPERTY IMPORTED_IMPLIB ${CMAKE_CURRENT_SOURCE_DIR}/SFMLDebug/lib/sfml-audio-d.lib)
 
add_library(sfml-graphics-d SHARED IMPORTED)
 set_property(TARGET sfml-graphics-d PROPERTY IMPORTED_LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/SFMLDebug/lib/sfml-graphics-d-2.dll)
 set_property(TARGET sfml-graphics-d PROPERTY IMPORTED_IMPLIB ${CMAKE_CURRENT_SOURCE_DIR}/SFMLDebug/lib/sfml-graphics-d.lib)

add_library(sfml-window-d SHARED IMPORTED)
 set_property(TARGET sfml-window-d PROPERTY IMPORTED_LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/SFMLDebug/lib/sfml-window-d-2.dll)
 set_property(TARGET sfml-window-d PROPERTY IMPORTED_IMPLIB ${CMAKE_CURRENT_SOURCE_DIR}/SFMLDebug/lib/sfml-window-d.lib)

add_library(sfml-main-d SHARED IMPORTED)
 set_property(TARGET sfml-main-d PROPERTY IMPORTED_IMPLIB ${CMAKE_CURRENT_SOURCE_DIR}/SFMLDebug/lib/sfml-main-d.lib)

add_library(sfml-system-d SHARED IMPORTED)
 set_property(TARGET sfml-system-d PROPERTY IMPORTED_LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/SFMLDebug/lib/sfml-system-d-2.dll)
 set_property(TARGET sfml-system-d PROPERTY IMPORTED_IMPLIB ${CMAKE_CURRENT_SOURCE_DIR}/SFMLDebug/lib/sfml-system-d.lib)

add_library(sfml-network-d SHARED IMPORTED)
 set_property(TARGET sfml-network-d PROPERTY IMPORTED_LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/SFMLDebug/lib/sfml-network-d-2.dll)
 set_property(TARGET sfml-network-d PROPERTY IMPORTED_IMPLIB ${CMAKE_CURRENT_SOURCE_DIR}/SFMLDebug/lib/sfml-network-d.lib)


target_link_libraries(
  R-TypeClient
 # sfml-audio
 # sfml-graphics
 # sfml-window
 # sfml-main
 # sfml-system
  #sfml-network

  sfml-audio-d
  sfml-graphics-d
  sfml-window-d
  sfml-main-d
  sfml-system-d
  sfml-network-d
)

if (UNIX)

add_custom_target(
fclean
COMMAND rm -rf CMakeFiles/ CMakeCache.txt cmake_install.cmake install_manifest.txt Makefile server
)

endif (UNIX)
