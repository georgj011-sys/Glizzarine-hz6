# Test tools (Windows, macOS, Linux). Built from the main CMakeLists with -DGLZH_TESTS=ON.
#   cmake -B build-tests -DGLZH_TESTS=ON -DCMAKE_BUILD_TYPE=Release
#   cmake -B build-asan  -DGLZH_TESTS=ON -DGLZH_SANITIZE=ON -DCMAKE_BUILD_TYPE=RelWithDebInfo   (clang/gcc: ASan + UBSan, MSVC: ASan)
#   cmake --build build-tests --target qa fuzz t10 stress pitch bench2 screenshots

set(GLZH_ROOT ${CMAKE_CURRENT_SOURCE_DIR}/..)

# JUCE modules + plugin code compiled once into a static library, shared by all tools
add_library(glzh_testlib STATIC
    ${GLZH_ROOT}/PluginProcessor.cpp
    ${GLZH_ROOT}/PluginEditor.cpp)
target_include_directories(glzh_testlib PUBLIC ${GLZH_ROOT} ${CMAKE_CURRENT_SOURCE_DIR})
target_compile_definitions(glzh_testlib PUBLIC
    GLZH_NO_CFG=1
    JUCE_STANDALONE_APPLICATION=1
    JUCE_USE_CURL=0
    JUCE_WEB_BROWSER=0
    JUCE_MODAL_LOOPS_PERMITTED=1
    JucePlugin_Name="Glizzarine Harmonizer"
    JucePlugin_Build_VST3=0
    JucePlugin_Build_Standalone=0)
target_link_libraries(glzh_testlib
    PUBLIC
        juce::juce_audio_utils
        juce::juce_dsp
        juce::juce_recommended_config_flags)

foreach(t qa fuzz screenshots bounce soak)
    add_executable(${t} ${t}.cpp)
    target_link_libraries(${t} PRIVATE glzh_testlib)
endforeach()

# engine-only tools (no JUCE)
foreach(t t10 stress pitch bench2 glide)
    add_executable(${t} ${t}.cpp)
    target_include_directories(${t} PRIVATE ${GLZH_ROOT})
endforeach()
