# add test environment variables
set(TEST_ENV "")

# set up test environment for ASan, LSan, and UBSan
if("${CMAKE_BUILD_TYPE}" STREQUAL "Asan")
  set(TEST_ENV "ASAN_OPTIONS=abort_on_error=0")

  if(LSAN_AVAILABLE)
    string(CONCAT TEST_ENV "${TEST_ENV}"
      ":detect_leaks=1;LSAN_OPTIONS=suppressions=${CMAKE_CURRENT_SOURCE_DIR}/lsan.supp")
  endif()

  if(UBSAN_AVAILABLE)
    string(CONCAT TEST_ENV "${TEST_ENV};" "UBSAN_OPTIONS=print_stacktrace=1")
  endif()
endif()

macro(add_spam_test basename)
  add_executable(${basename} ${basename}.cc)
  target_link_libraries(${basename} spam GTest::gtest GTest::gtest_main)
  gtest_add_tests(TARGET ${basename} TEST_LIST ${basename}-tests)

  if(NOT TEST_ENV STREQUAL "")
    set_tests_properties(${${basename}-tests} PROPERTIES ENVIRONMENT "${TEST_ENV}")
  endif()
endmacro()

add_spam_test(example_test)
add_spam_test(fl_test)
add_spam_test(matrix_test)
