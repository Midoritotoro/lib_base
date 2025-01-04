set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOMOC ON)
 
set(QT_PRIVATE_SOURCES 
    ${LIB_BASE_TOP_LEVEL_DIR}/qt/widgets/PopupMenu.cpp

    ${LIB_BASE_TOP_LEVEL_DIR}/qt/widgets/Separator.cpp
    ${LIB_BASE_TOP_LEVEL_DIR}/qt/style/StyleWidgets.cpp

	${LIB_BASE_TOP_LEVEL_DIR}/qt/style/StyleCore.cpp
    ${LIB_BASE_TOP_LEVEL_DIR}/qt/style/StyleScale.cpp

    ${LIB_BASE_TOP_LEVEL_DIR}/qt/text/TextEntities.cpp
    ${LIB_BASE_TOP_LEVEL_DIR}/qt/text/TextEntities.h

    ${LIB_BASE_TOP_LEVEL_DIR}/qt/text/TextWord.cpp
    ${LIB_BASE_TOP_LEVEL_DIR}/qt/text/TextWord.h
    
    ${LIB_BASE_TOP_LEVEL_DIR}/qt/text/TextDrawUtility.cpp
    ${LIB_BASE_TOP_LEVEL_DIR}/qt/text/TextDrawUtility.h

    ${LIB_BASE_TOP_LEVEL_DIR}/qt/text/TextClickHandlers.cpp

    ${LIB_BASE_TOP_LEVEL_DIR}/qt/text/String.cpp
    ${LIB_BASE_TOP_LEVEL_DIR}/qt/text/TextBidiAlgorithm.h

    ${LIB_BASE_TOP_LEVEL_DIR}/qt/text/TextStackEngine.cpp
    ${LIB_BASE_TOP_LEVEL_DIR}/qt/text/TextStackEngine.h

    ${LIB_BASE_TOP_LEVEL_DIR}/qt/text/WordParser.cpp
    ${LIB_BASE_TOP_LEVEL_DIR}/qt/text/WordParser.h

    ${LIB_BASE_TOP_LEVEL_DIR}/qt/text/BlockParser.cpp
    ${LIB_BASE_TOP_LEVEL_DIR}/qt/text/BlockParser.h

    ${LIB_BASE_TOP_LEVEL_DIR}/qt/text/TextRenderer.cpp
    ${LIB_BASE_TOP_LEVEL_DIR}/qt/text/TextRenderer.h

    ${LIB_BASE_TOP_LEVEL_DIR}/qt/text/TextUtility.cpp
    ${LIB_BASE_TOP_LEVEL_DIR}/qt/text/TextUtility.h

    ${LIB_BASE_TOP_LEVEL_DIR}/qt/text/TextBlock.cpp
    ${LIB_BASE_TOP_LEVEL_DIR}/qt/text/TextBlock.h

    ${LIB_BASE_TOP_LEVEL_DIR}/qt/style/StyleFont.cpp
   
	${LIB_BASE_TOP_LEVEL_DIR}/qt/common/ClickHandler.cpp
	
    ${LIB_BASE_TOP_LEVEL_DIR}/qt/common/BasicClickHandlers.cpp
    ${LIB_BASE_TOP_LEVEL_DIR}/qt/common/Timer.cpp

	${LIB_BASE_TOP_LEVEL_DIR}/qt/widgets/AbstractFlatButton.cpp
    ${LIB_BASE_TOP_LEVEL_DIR}/qt/widgets/FlatButton.cpp

	${LIB_BASE_TOP_LEVEL_DIR}/qt/widgets/ContinuousScroll.cpp
	${LIB_BASE_TOP_LEVEL_DIR}/qt/widgets/ContinuousScroll.h

    ${LIB_BASE_TOP_LEVEL_DIR}/qt/widgets/FlatLabel.cpp

	${LIB_BASE_TOP_LEVEL_DIR}/qt/widgets/ScrollArea.cpp
    ${LIB_BASE_TOP_LEVEL_DIR}/qt/widgets/EnhancedSlider.cpp
)

set(QT_PUBLIC_HEADERS 
    ${LIB_BASE_TOP_LEVEL_DIR}/include/qt/InvokeQueued.h
    ${LIB_BASE_TOP_LEVEL_DIR}/include/qt/StyleWidgets.h

    ${LIB_BASE_TOP_LEVEL_DIR}/include/qt/EnhancedSlider.h
    ${LIB_BASE_TOP_LEVEL_DIR}/include/qt/ScrollArea.h

    ${LIB_BASE_TOP_LEVEL_DIR}/include/qt/FlatLabel.h
    ${LIB_BASE_TOP_LEVEL_DIR}/include/qt/FlatButton.h

    ${LIB_BASE_TOP_LEVEL_DIR}/include/qt/AbstractFlatButton.h
    ${LIB_BASE_TOP_LEVEL_DIR}/include/qt/BasicClickHandlers.h

    ${LIB_BASE_TOP_LEVEL_DIR}/include/qt/ClickHandler.h
    ${LIB_BASE_TOP_LEVEL_DIR}/include/qt/WidgetsHider.h

    ${LIB_BASE_TOP_LEVEL_DIR}/include/qt/StyleFont.h
    ${LIB_BASE_TOP_LEVEL_DIR}/include/qt/StyleTypes.h

    ${LIB_BASE_TOP_LEVEL_DIR}/include/qt/Types.h
    ${LIB_BASE_TOP_LEVEL_DIR}/include/qt/String.h

    ${LIB_BASE_TOP_LEVEL_DIR}/include/qt/StyleScale.h
    ${LIB_BASE_TOP_LEVEL_DIR}/include/qt/StyleCore.h

    ${LIB_BASE_TOP_LEVEL_DIR}/include/qt/MainQueueProcessor.cpp

    ${LIB_BASE_TOP_LEVEL_DIR}/include/qt/PopupMenu.h
    ${LIB_BASE_TOP_LEVEL_DIR}/include/qt/Separator.h

    ${LIB_BASE_TOP_LEVEL_DIR}/include/qt/Timer.h
     ${LIB_BASE_TOP_LEVEL_DIR}/include/qt/TextClickHandlers.h
)

include(CMakePackageConfigHelpers)
include(GenerateExportHeader)

add_library(BaseQtUtility ${PUBLIC_HEADERS})
add_library(BaseQtUtility::BaseQtUtility ALIAS BaseQtUtility)

target_sources(BaseQtUtility PRIVATE ${PRIVATE_SOURCES})

set_target_properties(
    BaseQtUtility PROPERTIES
    CMAKE_CXX_STANDARD 23
    CMAKE_CXX_STANDARD_REQUIRED ON
    EXPORT_NAME BaseQtUtility
    OUTPUT_NAME BaseQtUtility
)

target_include_directories(BaseQtUtility
    PUBLIC
         "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>"
)

target_include_directories(BaseQtUtility
    PUBLIC
        "$<INSTALL_INTERFACE:include>"
)

target_include_directories(BaseQtUtility
    PUBLIC
        "\$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>"
)

target_include_directories(BaseQtUtility
    PUBLIC
        "\$<BUILD_INTERFACE:${PROJECT_BINARY_DIR}/export>"
)