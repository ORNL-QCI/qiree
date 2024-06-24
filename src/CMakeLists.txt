set(LIBRARY_NAME xacc-qir-compiler)

file(GLOB HEADERS *.hpp)
file(GLOB SRC *.cpp)

usfunctiongetresourcesource(TARGET ${LIBRARY_NAME} OUT SRC)
usfunctiongeneratebundleinit(TARGET ${LIBRARY_NAME} OUT SRC)

add_library(${LIBRARY_NAME} SHARED ${SRC})

set(_bundle_name xacc_qir_compiler)

set_target_properties(${LIBRARY_NAME}
                    PROPERTIES COMPILE_DEFINITIONS
                                US_BUNDLE_NAME=${_bundle_name}
                                US_BUNDLE_NAME
                                ${_bundle_name})

usfunctionembedresources(TARGET
                        ${LIBRARY_NAME}
                        WORKING_DIRECTORY
                        ${CMAKE_CURRENT_SOURCE_DIR}
                        FILES
                        manifest.json)

target_include_directories(${LIBRARY_NAME} PUBLIC)
target_link_libraries(${LIBRARY_NAME} PRIVATE xacc::xacc)

xacc_configure_plugin_rpath(${LIBRARY_NAME})

install(FILES ${HEADERS} DESTINATION include)
install(TARGETS ${LIBRARY_NAME} DESTINATION ${CMAKE_INSTALL_PREFIX}/plugins)
