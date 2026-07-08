# Generated Cmake Pico project file

cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Initialise pico_sdk from installed location
# (note this can come from environment, CMake cache etc)

# == DO NOT EDIT THE FOLLOWING LINES for the Raspberry Pi Pico VS Code Extension to work ==
if(WIN32)
    set(USERHOME $ENV{USERPROFILE})
else()
    set(USERHOME $ENV{HOME})
endif()
set(sdkVersion 2.3.0)
set(toolchainVersion 14_2_Rel1)
set(picotoolVersion 2.3.0)
set(picoVscode ${USERHOME}/.pico-sdk/cmake/pico-vscode.cmake)
if (EXISTS ${picoVscode})
    include(${picoVscode})
endif()
# ====================================================================================
set(PICO_BOARD pico CACHE STRING "Board type")

# Pull in Raspberry Pi Pico SDK (must be before project)
include(pico_sdk_import.cmake)

project(U2HTS C CXX ASM)

add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/u2hts_core)

set(U2HTS_ENABLE_FREERTOS OFF CACHE BOOL "Enable FreeRTOS support")

if(U2HTS_ENABLE_FREERTOS)
    include(${CMAKE_CURRENT_LIST_DIR}/FreeRTOS-Kernel/portable/ThirdParty/GCC/RP2040/FreeRTOS_Kernel_import.cmake)
    add_library(freertos INTERFACE)
    target_include_directories(freertos INTERFACE 
        ${CMAKE_CURRENT_LIST_DIR}/FreeRTOS-Kernel/include
        ${CMAKE_CURRENT_LIST_DIR}/FreeRTOS-Kernel/portable/ThirdParty/GCC/RP2040/include
        ${CMAKE_CURRENT_LIST_DIR}/include
    )

    target_link_libraries(freertos INTERFACE 
        pico_async_context_freertos
        FreeRTOS-Kernel-Static
    )

    target_compile_definitions(freertos INTERFACE
        U2HTS_ENABLE_FREERTOS
    )
    target_link_libraries(u2hts_core_settings INTERFACE freertos)

    set(TINYUSB_OPT_OS OPT_OS_FREERTOS)
endif()

# Initialise the Raspberry Pi Pico SDK
pico_sdk_init()

# Default hid report descriptor contains 10 touch points
# Below is an example of how to override it with 5 touch points.

# set(U2HTS_MAX_TPS 5)
# u2hts_generate_hid_report_descriptor()

target_compile_definitions(u2hts_core_settings INTERFACE
    U2HTS_LOG_LEVEL=U2HTS_LOG_LEVEL_INFO
    U2HTS_ENABLE_LED
    # U2HTS_ENABLE_PERSISTENT_CONFIG
    # U2HTS_ENABLE_KEY
    U2HTS_CONFIG_TIMEOUT=5000
    # U2HTS_ENABLE_COMPACT_REPORT
)

set(TOUCH_CONTROLLERS
    ${CMAKE_CURRENT_LIST_DIR}/u2hts_touch_controllers/gt9xx.c
    ${CMAKE_CURRENT_LIST_DIR}/u2hts_touch_controllers/ft54x6.c
    ${CMAKE_CURRENT_LIST_DIR}/u2hts_touch_controllers/rmi_common.c
    ${CMAKE_CURRENT_LIST_DIR}/u2hts_touch_controllers/rmi_f11.c
    ${CMAKE_CURRENT_LIST_DIR}/u2hts_touch_controllers/cst8xx.c
    ${CMAKE_CURRENT_LIST_DIR}/u2hts_touch_controllers/chsc6540.c
    ${CMAKE_CURRENT_LIST_DIR}/u2hts_touch_controllers/goodix_normandy.c
)

set(SOURCES 
    ${CMAKE_CURRENT_LIST_DIR}/src/u2hts_rp2.c
    ${CMAKE_CURRENT_LIST_DIR}/u2hts_main.c
)

# Add executable. Default name is the project name, version 0.1
add_executable(U2HTS ${SOURCES} ${TOUCH_CONTROLLERS})

find_package(Git QUIET)
if(GIT_FOUND)
    execute_process(
        COMMAND ${GIT_EXECUTABLE} describe --tags --dirty --always
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        OUTPUT_VARIABLE U2HTS_RP2_GIT_SHA
        OUTPUT_STRIP_TRAILING_WHITESPACE
        ERROR_QUIET
    )
else()
    set(U2HTS_RP2_GIT_SHA "UNKNOWN")
endif()

pico_set_program_name(U2HTS "U2HTS")
pico_set_program_version(U2HTS ${U2HTS_RP2_GIT_SHA})

# Modify the below lines to enable/disable output over UART/USB
pico_enable_stdio_uart(U2HTS 1)
pico_enable_stdio_usb(U2HTS 0)

# Add the standard library to the build
target_link_libraries(U2HTS PUBLIC
    pico_stdlib
)

# Add the standard include files to the build
target_include_directories(U2HTS PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}
  ${CMAKE_CURRENT_LIST_DIR}/include
)

# print memory usage after linking
target_link_options(U2HTS PRIVATE
    -Wl,--print-memory-usage
)

# Add any user requested libraries
target_link_libraries(U2HTS PUBLIC
    hardware_i2c
    pico_unique_id
    tinyusb_device
    tinyusb_board
    u2hts_core
)

target_compile_options(U2HTS PRIVATE 
    -Wall
    -Wno-format
)

if(PICO_PLATFORM MATCHES "rp2350")
    pico_set_linker_script(U2HTS ${CMAKE_CURRENT_LIST_DIR}/memmap_rp2350.ld)
elseif(PICO_PLATFORM MATCHES "rp2040")
    pico_set_linker_script(U2HTS ${CMAKE_CURRENT_LIST_DIR}/memmap_rp2040.ld)
else()
    message(Unknown platform: ${PICO_PLATFORM})
endif()

pico_add_extra_outputs(U2HTS)
pico_set_program_url(U2HTS "https://github.com/U2HTS/U2HTS")
