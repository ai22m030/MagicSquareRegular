########################################################################################################################
### MAGIC SQUARE APPLICATION
########################################################################################################################

add_executable(${MAGIC_SQUARE_PROJECT_NAME}
        ${MAGIC_SQUARE_SRC_DIR}/main.cpp
        ${MAGIC_SQUARE_SRC_DIR}/program_options.cpp
        ${MAGIC_SQUARE_SRC_DIR}/magic_square.cpp)

target_compile_features(${MAGIC_SQUARE_PROJECT_NAME} PUBLIC
        cxx_std_17)

target_include_directories(${MAGIC_SQUARE_PROJECT_NAME} PUBLIC
        ${MAGIC_SQUARE_INC_DIR})