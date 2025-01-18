set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(LIB_BASE_QT_ENABLE_FONTS "¬ключить кастомные шрифты" ON)
set(LIB_BASE_QT_ENABLE_FONTS ON)

set(QT_PRIVATE_SOURCES 
    ${PROJECT_SOURCE_DIR}/src/qt/ui/PopupMenu.cpp

    ${PROJECT_SOURCE_DIR}/src/qt/ui/Separator.cpp
    ${PROJECT_SOURCE_DIR}/src/qt/style/StyleWidgets.cpp

	${PROJECT_SOURCE_DIR}/src/qt/style/StyleCore.cpp
    ${PROJECT_SOURCE_DIR}/src/qt/style/StyleScale.cpp

    ${PROJECT_SOURCE_DIR}/src/qt/text/TextEntities.cpp
    ${PROJECT_SOURCE_DIR}/src/qt/text/TextEntities.h

    ${PROJECT_SOURCE_DIR}/src/qt/text/TextWord.cpp
    ${PROJECT_SOURCE_DIR}/src/qt/text/TextWord.h
    
    ${PROJECT_SOURCE_DIR}/src/qt/text/TextDrawUtility.cpp
    ${PROJECT_SOURCE_DIR}/src/qt/text/TextDrawUtility.h

    ${PROJECT_SOURCE_DIR}/src/qt/text/TextClickHandlers.cpp

    ${PROJECT_SOURCE_DIR}/src/qt/text/String.cpp
    ${PROJECT_SOURCE_DIR}/src/qt/text/TextBidiAlgorithm.h

    ${PROJECT_SOURCE_DIR}/src/qt/text/TextStackEngine.cpp
    ${PROJECT_SOURCE_DIR}/src/qt/text/TextStackEngine.h

    ${PROJECT_SOURCE_DIR}/src/qt/text/WordParser.cpp
    ${PROJECT_SOURCE_DIR}/src/qt/text/WordParser.h

    ${PROJECT_SOURCE_DIR}/src/qt/text/BlockParser.cpp
    ${PROJECT_SOURCE_DIR}/src/qt/text/BlockParser.h

    ${PROJECT_SOURCE_DIR}/src/qt/text/TextRenderer.cpp
    ${PROJECT_SOURCE_DIR}/src/qt/text/TextRenderer.h

    ${PROJECT_SOURCE_DIR}/src/qt/text/TextUtility.cpp

    ${PROJECT_SOURCE_DIR}/src/qt/text/TextBlock.cpp
    ${PROJECT_SOURCE_DIR}/src/qt/text/TextBlock.h

    ${PROJECT_SOURCE_DIR}/src/qt/style/StyleFont.cpp
   
	${PROJECT_SOURCE_DIR}/src/qt/common/ClickHandler.cpp
	
    ${PROJECT_SOURCE_DIR}/src/qt/common/BasicClickHandlers.cpp
    ${PROJECT_SOURCE_DIR}/src/qt/common/Timer.cpp

    ${PROJECT_SOURCE_DIR}/src/qt/common/MainQueueProcessor.cpp

	${PROJECT_SOURCE_DIR}/src/qt/ui/AbstractFlatButton.cpp
    ${PROJECT_SOURCE_DIR}/src/qt/ui/FlatButton.cpp

	${PROJECT_SOURCE_DIR}/src/qt/ui/ContinuousScroll.cpp
    ${PROJECT_SOURCE_DIR}/src/qt/ui/FlatLabel.cpp

	${PROJECT_SOURCE_DIR}/src/qt/ui/ScrollArea.cpp
    ${PROJECT_SOURCE_DIR}/src/qt/ui/EnhancedSlider.cpp

    ${PROJECT_SOURCE_DIR}/src/qt/ui/animations/AnimationBase.cpp
    ${PROJECT_SOURCE_DIR}/src/qt/ui/animations/AnimationManager.cpp

    ${PROJECT_SOURCE_DIR}/src/qt/ui/animations/OpacityAnimation.cpp

    ${PROJECT_SOURCE_DIR}/src/qt/ui/animations/HorizontalGrowthAnimation.cpp
    ${PROJECT_SOURCE_DIR}/src/qt/ui/animations/VerticalGrowthAnimation.cpp

    ${PROJECT_SOURCE_DIR}/src/qt/ui/animations/CombinedGrowthAnimation.cpp
    ${PROJECT_SOURCE_DIR}/src/qt/ui/effects/BlurBehind.cpp

    ${PROJECT_SOURCE_DIR}/src/qt/ui/effects/OpenGLBlur.cpp

    ${PROJECT_SOURCE_DIR}/src/qt/ui/effects/shaders/dual_kawase_down.frag
    ${PROJECT_SOURCE_DIR}/src/qt/ui/effects/shaders/dual_kawase_up.frag
    ${PROJECT_SOURCE_DIR}/src/qt/ui/effects/shaders/simple.vert
)

set(QT_PUBLIC_HEADERS 
    ${PROJECT_SOURCE_DIR}/include/base/qt/ui/BaseWidget.h   
    ${PROJECT_SOURCE_DIR}/include/base/qt/ui/Utility.h   

    ${PROJECT_SOURCE_DIR}/include/base/qt/common/UniqueQPtr.h
    ${PROJECT_SOURCE_DIR}/include/base/qt/common/Size.h

    ${PROJECT_SOURCE_DIR}/include/base/qt/common/InvokeQueued.h
    ${PROJECT_SOURCE_DIR}/include/base/qt/common/BasicClickHandlers.h

    ${PROJECT_SOURCE_DIR}/include/base/qt/common/ClickHandler.h
    ${PROJECT_SOURCE_DIR}/include/base/qt/common/WidgetsHider.h

    ${PROJECT_SOURCE_DIR}/include/base/qt/common/Types.h
        
    ${PROJECT_SOURCE_DIR}/include/base/qt/common/Timer.h
    ${PROJECT_SOURCE_DIR}/include/base/qt/style/StyleWidgets.h

    ${PROJECT_SOURCE_DIR}/include/base/qt/style/StyleFont.h
    ${PROJECT_SOURCE_DIR}/include/base/qt/style/StyleTypes.h

    ${PROJECT_SOURCE_DIR}/include/base/qt/style/StyleScale.h
    ${PROJECT_SOURCE_DIR}/include/base/qt/style/StyleCore.h

    ${PROJECT_SOURCE_DIR}/include/base/qt/ui/EnhancedSlider.h
    ${PROJECT_SOURCE_DIR}/include/base/qt/ui/ScrollArea.h

    ${PROJECT_SOURCE_DIR}/include/base/qt/ui/FlatLabel.h
    ${PROJECT_SOURCE_DIR}/include/base/qt/ui/FlatButton.h

    ${PROJECT_SOURCE_DIR}/include/base/qt/ui/AbstractFlatButton.h

    ${PROJECT_SOURCE_DIR}/include/base/qt/ui/PopupMenu.h
    ${PROJECT_SOURCE_DIR}/include/base/qt/ui/Separator.h

    ${PROJECT_SOURCE_DIR}/include/base/qt/text/String.h
    ${PROJECT_SOURCE_DIR}/include/base/qt/text/TextClickHandlers.h

    ${PROJECT_SOURCE_DIR}/include/base/qt/text/TextUtility.h

    ${PROJECT_SOURCE_DIR}/include/base/qt/ui/ContinuousScroll.h

    ${PROJECT_SOURCE_DIR}/include/base/qt/ui/animations/AnimationBase.h
    ${PROJECT_SOURCE_DIR}/include/base/qt/ui/animations/OpacityAnimation.h

    ${PROJECT_SOURCE_DIR}/include/base/qt/ui/animations/AnimationManager.h

    ${PROJECT_SOURCE_DIR}/include/base/qt/ui/animations/HorizontalGrowthAnimation.h
    ${PROJECT_SOURCE_DIR}/include/base/qt/ui/animations/VerticalGrowthAnimation.h

    ${PROJECT_SOURCE_DIR}/include/base/qt/ui/animations/CombinedGrowthAnimation.h
    ${PROJECT_SOURCE_DIR}/include/base/qt/ui/effects/BlurBehind.h

    ${PROJECT_SOURCE_DIR}/include/base/qt/ui/effects/OpenGLBlur.h
    ${PROJECT_SOURCE_DIR}/include/base/qt/ui/effects/OpenGLVertex.h

    ${PROJECT_SOURCE_DIR}/include/base/qt/ui/PainterHqEnabler.h
)


include(CMakePackageConfigHelpers)
include(GenerateExportHeader)

add_library(lib_base_qt ${QT_PUBLIC_HEADERS})

set_target_properties(
    lib_base_qt PROPERTIES
    CXX_STANDARD 23
    CXX_STANDARD_REQUIRED ON
    AUTOMOC ON
    AUTORCC ON
    EXPORT_NAME lib_base_qt
    OUTPUT_NAME lib_base_qt
)

target_compile_features(lib_base_qt PUBLIC cxx_std_23)

include(${PROJECT_SOURCE_DIR}/cmake/Util_FindQt.cmake)
include(${PROJECT_SOURCE_DIR}/cmake/Util_FindGSL.cmake)

find_ms_gsl(lib_base_qt)
find_qt(lib_base_qt)

target_sources(lib_base_qt PRIVATE ${QT_PRIVATE_SOURCES})

if(LIB_BASE_QT_ENABLE_FONTS)
    target_sources(lib_base_qt PUBLIC ${PROJECT_SOURCE_DIR}/resources/fonts/fonts.qrc)
    target_compile_definitions(lib_base_qt
        PUBLIC
            LIB_BASE_QT_ENABLE_FONTS
            LIB_BASE_SHADERS="${PROJECT_SOURCE_DIR}/src/qt/ui/effects/shaders/"
    )
endif()

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