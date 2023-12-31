# -*- explicit-buffer-name: "CMakeLists.txt<M1-MOBJ/8-10>" -*-
#
# To display executed commands, use:
#     > cmake -DCMAKE_VERBOSE_MAKEFILE:STRING=YES ../src

cmake_minimum_required(VERSION 2.8.0)
project(SCHEMATIC)

set (CMAKE_CXX_FLAGS            "-Wall -g" CACHE STRING "C++ Compiler Release options." FORCE)
set (CMAKE_INSTALL_PREFIX       "../install" )

find_package(LibXml2 REQUIRED)
find_package(Qt5Core       REQUIRED)
find_package(Qt5Gui        REQUIRED)
find_package(Qt5Widgets    REQUIRED)
#include(${QT_USE_FILE})

   include_directories( ${SCHEMATIC_SOURCE_DIR}
                        ${Qt5Core_INCLUDE_DIRS}
                        ${Qt5Gui_INCLUDE_DIRS}
                        ${Qt5Widgets_INCLUDE_DIRS}
                        ${LIBXML2_INCLUDE_DIR}
                      )

                   set( includes     Indentation.h
                                     XmlUtil.h 
                                     Point.h
                                     Box.h
                                     Term.h
                                     Net.h
                                     Instance.h
                                     Node.h
                                     Line.h
                                     Shape.h
                                     Symbol.h
                                     Cell.h
                      )

                   set( mocIncludes  SaveCellDialog.h
                                     OpenCellDialog.h
                                     CellWidget.h
                                     CellViewer.h
                                     InstancesWidget.h
                                     InstancesModel.h
                                     CellsLib.h
                                     CellsModel.h
                      )

                   set( cpps         Indentation.cpp
                                     XmlUtil.cpp 
                                     Point.cpp
                                     Box.cpp
                                     Term.cpp
                                     Net.cpp
                                     Instance.cpp
                                     Node.cpp
                                     Line.cpp
                                     Shape.cpp
                                     Symbol.cpp
                                     Cell.cpp
                                     SaveCellDialog.cpp
                                     OpenCellDialog.cpp
                                     CellWidget.cpp
                                     CellViewer.cpp
                                     InstancesWidget.cpp
                                     InstancesModel.cpp
                                     CellsLib.cpp
                                     CellsModel.cpp
                                     Main.cpp
                      )

          qt5_wrap_cpp( mocCpps      ${mocIncludes} )

        add_executable( tme810       ${cpps} ${mocCpps} )
 target_link_libraries( tme810       ${Qt5Core_LIBRARIES} ${Qt5Gui_LIBRARIES} ${Qt5Widgets_LIBRARIES} ${LIBXML2_LIBRARIES} )
               install( TARGETS      tme810                      DESTINATION bin )
               install( FILES        ${includes} ${mocIncludes}  DESTINATION include )
