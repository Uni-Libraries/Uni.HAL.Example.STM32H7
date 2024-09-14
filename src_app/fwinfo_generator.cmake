# This script is executed at build time by add_custom_command
# to generate the app_fwinfo.c file.

find_package(Git REQUIRED)

# Get Git information
execute_process(COMMAND ${GIT_EXECUTABLE} describe --tags --always --dirty=-modified
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    OUTPUT_VARIABLE GIT_VERSION
    OUTPUT_STRIP_TRAILING_WHITESPACE)
execute_process(COMMAND ${GIT_EXECUTABLE} rev-parse HEAD
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    OUTPUT_VARIABLE GIT_COMMIT_SHA
    OUTPUT_STRIP_TRAILING_WHITESPACE)
execute_process(COMMAND ${GIT_EXECUTABLE} rev-parse --abbrev-ref HEAD
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    OUTPUT_VARIABLE GIT_BRANCH
    OUTPUT_STRIP_TRAILING_WHITESPACE)

# Get build timestamp
string(TIMESTAMP BUILD_TIMESTAMP "%Y-%m-%dT%H:%M:%SZ" UTC)

# Configure the file
# Variables like PROJECT_NAME are passed via -D arguments
# from the add_custom_command in the main CMakeLists.txt
# and are used to replace @VAR@ placeholders in the template.
configure_file(
    ${FWINFO_TEMPLATE_IN}
    ${FWINFO_GENERATED_C}
    @ONLY
)