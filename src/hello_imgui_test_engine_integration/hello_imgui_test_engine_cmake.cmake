if(POLICY CMP0079)
    cmake_policy(SET CMP0079 NEW) # target_link_libraries() allows use with targets in other directories.
endif()


# Add imgui_test_engine lib with sources in imgui_test_engine/imgui_test_engine
function(_add_imgui_test_engine_lib)
    set(source_folder ${IMGUI_TEST_ENGINE_BASEPATH}/imgui_test_engine)
    file(GLOB_RECURSE sources ${source_folder}/*.h ${source_folder}/*.cpp)
    add_library(imgui_test_engine ${sources})
    target_include_directories(imgui_test_engine PUBLIC ${source_folder}/..)
endfunction()


# ImGui uses imconfig from imconfig_with_test_engine.h (with options for imgui_test_engine)
function(_configure_imgui_with_test_engine)
    target_compile_definitions(imgui PUBLIC IMGUI_USER_CONFIG="${CMAKE_CURRENT_FUNCTION_LIST_DIR}/imconfig_with_test_engine.h")
    # Link imgui_test_engine with imgui
    target_link_libraries(imgui_test_engine PUBLIC imgui)
    # any App built with ImGui should now also link with imgui_test_engine
    target_link_libraries(imgui PUBLIC imgui_test_engine)
endfunction()


# Add integration into HelloImGui
function(_add_hello_imgui_test_engine_integration)
    target_compile_definitions(hello_imgui PUBLIC HELLOIMGUI_WITH_TEST_ENGINE)
    target_sources(hello_imgui PUBLIC
        ${CMAKE_CURRENT_FUNCTION_LIST_DIR}/test_engine_integration.cpp
        ${CMAKE_CURRENT_FUNCTION_LIST_DIR}/test_engine_integration.h
        )
    target_include_directories(hello_imgui PUBLIC ${CMAKE_CURRENT_FUNCTION_LIST_DIR}/..)
endfunction()


function(add_imgui_test_engine)
    _add_imgui_test_engine_lib()
    _configure_imgui_with_test_engine()
    _add_hello_imgui_test_engine_integration()
endfunction()