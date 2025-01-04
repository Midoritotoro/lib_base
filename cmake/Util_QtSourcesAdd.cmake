include(cmake/FindQt)

macro(addQtSources)
    find_qt()
    add_subdirectory(${LIB_BASE_TOP_LEVEL_DIR}/src/qt) 
endmacro()