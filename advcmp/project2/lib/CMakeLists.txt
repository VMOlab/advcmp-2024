set(PASS_PLUGINS
    SimpleSCCP
)
set(SimpleSCCP_SOURCES
    SimpleSCCP.cpp
    SparseDataflowFramework.cpp
)

foreach(plugin ${PASS_PLUGINS})
    add_library(${plugin} SHARED ${${plugin}_SOURCES})

    target_include_directories(
        ${plugin}
        PRIVATE
        "${CMAKE_CURRENT_SOURCE_DIR}/../include")

    target_link_libraries(${plugin}
        PRIVATE
        LLVM)
    
    set_target_properties(${plugin}
        PROPERTIES
        LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
endforeach()
