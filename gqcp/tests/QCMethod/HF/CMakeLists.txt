list(APPEND test_target_sources
    ${CMAKE_CURRENT_SOURCE_DIR}/QCMethod_RHF_test.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/RHFSCFEnvironment_test.cpp
)

set(test_target_sources ${test_target_sources} PARENT_SCOPE)
