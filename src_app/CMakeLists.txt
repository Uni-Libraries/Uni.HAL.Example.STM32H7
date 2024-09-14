
#
# App
#

# --- Firmware Info Generation ---
set(FWINFO_GENERATED_C ${CMAKE_CURRENT_BINARY_DIR}/app_fwinfo.c)

# Add dependency on the current git commit to force regeneration when the commit changes.
set(GIT_HEAD_FILE ${CMAKE_SOURCE_DIR}/.git/HEAD)
if(EXISTS ${GIT_HEAD_FILE})
    file(READ ${GIT_HEAD_FILE} GIT_HEAD_CONTENTS)
    if(GIT_HEAD_CONTENTS MATCHES "ref: (.*)")
        set(GIT_REF_FILE ${CMAKE_SOURCE_DIR}/.git/${CMAKE_MATCH_1})
    else()
        set(GIT_REF_FILE ${GIT_HEAD_FILE})
    endif()
else()
    set(GIT_REF_FILE "")
endif()


add_custom_command(
    OUTPUT ${FWINFO_GENERATED_C}
    COMMAND ${CMAKE_COMMAND}
        -D FWINFO_TEMPLATE_IN=${CMAKE_CURRENT_LIST_DIR}/app_fwinfo.c.in
        -D FWINFO_GENERATED_C=${FWINFO_GENERATED_C}
        -D OX_TARGET=${OX_TARGET}
        -D CMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
        -D CMAKE_C_COMPILER_ID=${CMAKE_C_COMPILER_ID}
        -D CMAKE_C_COMPILER_VERSION=${CMAKE_C_COMPILER_VERSION}
        -P ${CMAKE_CURRENT_LIST_DIR}/fwinfo_generator.cmake
    DEPENDS ${CMAKE_CURRENT_LIST_DIR}/app_fwinfo.c.in ${CMAKE_CURRENT_LIST_DIR}/fwinfo_generator.cmake ${GIT_REF_FILE}
    COMMENT "Generating fresh app_fwinfo.c"
)

add_custom_target(fwinfo_generator DEPENDS ${FWINFO_GENERATED_C})

# --- Main Application Target ---
uni_hal_add_executable(uni_hal_example_stm32h7_app)
add_dependencies(uni_hal_example_stm32h7_app fwinfo_generator)

target_sources(uni_hal_example_stm32h7_app PRIVATE
    app.c
    ${FWINFO_GENERATED_C}
    app_handlers.c
    app_hash.c
    app_key.c
)

target_link_libraries(uni_hal_example_stm32h7_app PRIVATE uni_hal_example_stm32h7_lib)
target_include_directories(uni_hal_example_stm32h7_app PRIVATE .)

if(NOT UNI_HAL_TARGET_MCU STREQUAL "PC")
    uni_hal_set_linker_file(uni_hal_example_stm32h7_app "${CMAKE_CURRENT_LIST_DIR}/app_linker.ld")
endif()


add_custom_command(
        TARGET uni_hal_example_stm32h7_app
        POST_BUILD WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}
        COMMAND Python::Interpreter
        ARGS ./inject_hash.py "${CMAKE_BINARY_DIR}/src_app/uni_hal_example_stm32h7_app.bin" "${CMAKE_BINARY_DIR}/src_app/uni_hal_example_stm32h7_app.elf" "app_key.c")
