list(APPEND test_target_sources
    ${CMAKE_CURRENT_SOURCE_DIR}/SpinResolvedFrozenONVBasis_test.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/SpinResolvedONVBasis_test.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/SpinResolvedSelectedONVBasis_test.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/SpinUnresolvedFrozenONVBasis_test.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/SpinUnresolvedONV_test.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/SpinUnresolvedONVBasis_test.cpp
)

set(test_target_sources ${test_target_sources} PARENT_SCOPE)
