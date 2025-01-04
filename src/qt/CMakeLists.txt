set(LIB_BASE_TOP_LEVEL_DIR "D:/lib_base/lib_base")

include(${LIB_BASE_TOP_LEVEL_DIR}/cmake/Util_FindQt.cmake)
find_qt()

set(QT_PRIVATE_SOURCES 
    ${LIB_BASE_TOP_LEVEL_DIR}/src/qt/ui/PopupMenu.cpp

    ${LIB_BASE_TOP_LEVEL_DIR}/src/qt/ui/Separator.cpp
    ${LIB_BASE_TOP_LEVEL_DIR}/src/qt/style/StyleWidgets.cpp

	${LIB_BASE_TOP_LEVEL_DIR}/src/qt/style/StyleCore.cpp
    ${LIB_BASE_TOP_LEVEL_DIR}/src/qt/style/StyleScale.cpp

    ${LIB_BASE_TOP_LEVEL_DIR}/src/qt/text/TextEntities.cpp
    ${LIB_BASE_TOP_LEVEL_DIR}/src/qt/text/TextEntities.h

    ${LIB_BASE_TOP_LEVEL_DIR}/src/qt/text/TextWord.cpp
    ${LIB_BASE_TOP_LEVEL_DIR}/src/qt/text/TextWord.h
    
    ${LIB_BASE_TOP_LEVEL_DIR}/src/qt/text/TextDrawUtility.cpp
    ${LIB_BASE_TOP_LEVEL_DIR}/src/qt/text/TextDrawUtility.h

    ${LIB_BASE_TOP_LEVEL_DIR}/src/qt/text/TextClickHandlers.cpp

    ${LIB_BASE_TOP_LEVEL_DIR}/src/qt/text/String.cpp
    ${LIB_BASE_TOP_LEVEL_DIR}/src/qt/text/TextBidiAlgorithm.h

    ${LIB_BASE_TOP_LEVEL_DIR}/src/qt/text/TextStackEngine.cpp
    ${LIB_BASE_TOP_LEVEL_DIR}/src/qt/text/TextStackEngine.h

    ${LIB_BASE_TOP_LEVEL_DIR}/src/qt/text/WordParser.cpp
    ${LIB_BASE_TOP_LEVEL_DIR}/src/qt/text/WordParser.h

    ${LIB_BASE_TOP_LEVEL_DIR}/src/qt/text/BlockParser.cpp
    ${LIB_BASE_TOP_LEVEL_DIR}/src/qt/text/BlockParser.h

    ${LIB_BASE_TOP_LEVEL_DIR}/src/qt/text/TextRenderer.cpp
    ${LIB_BASE_TOP_LEVEL_DIR}/src/qt/text/TextRenderer.h

    ${LIB_BASE_TOP_LEVEL_DIR}/src/qt/text/TextUtility.cpp
    ${LIB_BASE_TOP_LEVEL_DIR}/src/qt/text/TextUtility.h

    ${LIB_BASE_TOP_LEVEL_DIR}/src/qt/text/TextBlock.cpp
    ${LIB_BASE_TOP_LEVEL_DIR}/src/qt/text/TextBlock.h

    ${LIB_BASE_TOP_LEVEL_DIR}/src/qt/style/StyleFont.cpp
   
	${LIB_BASE_TOP_LEVEL_DIR}/src/qt/common/ClickHandler.cpp
	
    ${LIB_BASE_TOP_LEVEL_DIR}/src/qt/common/BasicClickHandlers.cpp
    ${LIB_BASE_TOP_LEVEL_DIR}/src/qt/common/Timer.cpp

    ${LIB_BASE_TOP_LEVEL_DIR}/src/qt/common/MainQueueProcessor.cpp

	${LIB_BASE_TOP_LEVEL_DIR}/src/qt/ui/AbstractFlatButton.cpp
    ${LIB_BASE_TOP_LEVEL_DIR}/src/qt/ui/FlatButton.cpp

	${LIB_BASE_TOP_LEVEL_DIR}/src/qt/ui/ContinuousScroll.cpp
    ${LIB_BASE_TOP_LEVEL_DIR}/src/qt/ui/FlatLabel.cpp

	${LIB_BASE_TOP_LEVEL_DIR}/src/qt/ui/ScrollArea.cpp
    ${LIB_BASE_TOP_LEVEL_DIR}/src/qt/ui/EnhancedSlider.cpp
)

set(QT_PUBLIC_HEADERS 
    ${LIB_BASE_TOP_LEVEL_DIR}/include/base/qt/common/InvokeQueued.h
    ${LIB_BASE_TOP_LEVEL_DIR}/include/base/qt/common/BasicClickHandlers.h

    ${LIB_BASE_TOP_LEVEL_DIR}/include/base/qt/common/ClickHandler.h
    ${LIB_BASE_TOP_LEVEL_DIR}/include/base/qt/common/WidgetsHider.h

    ${LIB_BASE_TOP_LEVEL_DIR}/include/base/qt/common/Types.h
        
    ${LIB_BASE_TOP_LEVEL_DIR}/include/base/qt/common/Timer.h
    ${LIB_BASE_TOP_LEVEL_DIR}/include/base/qt/style/StyleWidgets.h

    ${LIB_BASE_TOP_LEVEL_DIR}/include/base/qt/style/StyleFont.h
    ${LIB_BASE_TOP_LEVEL_DIR}/include/base/qt/style/StyleTypes.h

    ${LIB_BASE_TOP_LEVEL_DIR}/include/base/qt/style/StyleScale.h
    ${LIB_BASE_TOP_LEVEL_DIR}/include/base/qt/style/StyleCore.h

    ${LIB_BASE_TOP_LEVEL_DIR}/include/base/qt/ui/EnhancedSlider.h
    ${LIB_BASE_TOP_LEVEL_DIR}/include/base/qt/ui/ScrollArea.h

    ${LIB_BASE_TOP_LEVEL_DIR}/include/base/qt/ui/FlatLabel.h
    ${LIB_BASE_TOP_LEVEL_DIR}/include/base/qt/ui/FlatButton.h

    ${LIB_BASE_TOP_LEVEL_DIR}/include/base/qt/ui/AbstractFlatButton.h

    ${LIB_BASE_TOP_LEVEL_DIR}/include/base/qt/ui/PopupMenu.h
    ${LIB_BASE_TOP_LEVEL_DIR}/include/base/qt/ui/Separator.h

    ${LIB_BASE_TOP_LEVEL_DIR}/include/base/qt/text/String.h
    ${LIB_BASE_TOP_LEVEL_DIR}/include/base/qt/text/TextClickHandlers.h
    ${LIB_BASE_TOP_LEVEL_DIR}/include/base/qt/ui/ContinuousScroll.h
)

include(CMakePackageConfigHelpers)
include(GenerateExportHeader)

add_library(lib_base_qt ${QT_PUBLIC_HEADERS})

set_target_properties(
    lib_base_qt PROPERTIES
    CMAKE_CXX_STANDARD 23
    CMAKE_CXX_STANDARD_REQUIRED ON
    CMAKE_AUTOMOC ON
    CMAKE_AUTORCC ON
    EXPORT_NAME BaseQtUtility
    OUTPUT_NAME BaseQtUtility
)

target_sources(lib_base_qt PRIVATE ${QT_PRIVATE_SOURCES})

target_include_directories(lib_base_qt
    PUBLIC
         "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>"
)

target_include_directories(lib_base_qt
    PUBLIC
        "$<INSTALL_INTERFACE:include>"
)

target_include_directories(lib_base_qt
    PUBLIC
        "\$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>"
)

target_include_directories(lib_base_qt
    PUBLIC
        "\$<BUILD_INTERFACE:${PROJECT_BINARY_DIR}/export>"
)