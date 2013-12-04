cmake_minimum_required(VERSION 2.8)

project(APRICOT)

include(${APRICOT_SOURCE_DIR}/cmake/FindAPR.cmake)

include_directories(
    ${APR_INCLUDE_DIR}
    ${APRUTIL_INCLUDE_DIR}
)

add_executable(apricot apricot.cpp)

target_link_libraries(
    apricot
    ${APR_LIBRARIES}
    ${APRUTIL_LIBRARIES}
)
