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
set(sdkVersion 2.1.1)
set(toolchainVersion 14_2_Rel1)
set(picotoolVersion 2.1.1)
set(picoVscode ${USERHOME}/.pico-sdk/cmake/pico-vscode.cmake)
if (EXISTS ${picoVscode})
    include(${picoVscode})
endif()
# ====================================================================================
set(PICO_BOARD pico_w CACHE STRING "Board type")

# Pull in Raspberry Pi Pico SDK (must be before project)
include(pico_sdk_import.cmake)

project(MS5637_02BA03_BIBLIO C CXX ASM)

# Initialise the Raspberry Pi Pico SDK
pico_sdk_init()

# Add executable. Default name is the project name, version 0.1

add_executable(MS5637_02BA03_BIBLIO
    src/ms5637_02ba03/ms5637.c
    # src/main.c
    # src/main_exemplos/Ex1_leitura.c
    # src/main_exemplos/Ex2_barometro.c
    # src/main_exemplos/Ex3_varBarometrica.c
    # src/main_exemplos/Ex3.1_varBaroMediaMov.c
    # src/main_exemplos/Ex3.2_varBaroKalman.c
    # src/main_exemplos/Ex3.3_varHypsometrica.c
    # src/main_exemplos/Ex3.4_varHypsoKalman.c
    # src/main_exemplos/Ex4_ajustepressao.c

    src/rotina_test/test_ms5637.c
)

pico_set_program_name(MS5637_02BA03_BIBLIO "MS5637_02BA03_BIBLIO")
pico_set_program_version(MS5637_02BA03_BIBLIO "0.1")

# Modify the below lines to enable/disable output over UART/USB
pico_enable_stdio_uart(MS5637_02BA03_BIBLIO 1)
pico_enable_stdio_usb(MS5637_02BA03_BIBLIO 1)

# Add the standard library to the build
target_link_libraries(MS5637_02BA03_BIBLIO 
        pico_stdlib)

# Add the standard include files to the build
target_include_directories(MS5637_02BA03_BIBLIO PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}
)

# Add any user requested libraries
target_link_libraries(MS5637_02BA03_BIBLIO
        hardware_i2c
        )

pico_add_extra_outputs(MS5637_02BA03_BIBLIO)

