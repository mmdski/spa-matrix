add_library(spam
  fl.c
  gauss.c
  matrix.c)

macro(add_spam_exe basename)
  add_executable(${basename} ${basename}.c)
  target_link_libraries(${basename} spam)
endmacro()
