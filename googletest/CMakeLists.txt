# 'Google_test' is the subproject name
project(Google_tests)

# 'lib' is the folder with Google Test sources
add_subdirectory(lib)
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

# 'Google_Tests_run' is the target name
# 'test1.cpp test2.cpp' are source files with tests
set(cpu6502_SOURCE_FILES ../src/Computer.cpp ../src/memory/Memory.cpp ../src/cpu/CPU.cpp ../src/cpu/CPUexecute.cpp)
set(cpu6502_TEST_FILES ../test/loadRegisterATests.cpp ../test/loadRegisterXTests.cpp ../test/loadRegisterYTests.cpp ../test/storeRegisterATests.cpp ../test/storeRegisterXTests.cpp ../test/storeRegisterYTests.cpp ../test/jumpsAndSubroutinesTests.cpp ../test/transferTests.cpp ../test/stackInstructionsTests.cpp ../test/andInstructionTests.cpp ../test/eorInstructionTests.cpp ../test/oraInstructionTests.cpp)

add_executable(Google_Tests_run ${cpu6502_TEST_FILES} ${cpu6502_SOURCE_FILES})
target_link_libraries(Google_Tests_run gtest gtest_main)