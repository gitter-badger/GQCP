list(APPEND test_target_sources
    ${CMAKE_CURRENT_SOURCE_DIR}/BlockMatrix_test.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/BlockRankFourTensor_test.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Matrix_test.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/QCMatrix_test.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/QCRankFourTensor_test.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/SquareMatrix_test.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/SquareRankFourTensor_test.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Tensor_test.cpp
)

set(test_target_sources ${test_target_sources} PARENT_SCOPE)
