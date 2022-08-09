add_library(spam
  fl.c
  matrix.c)

macro(add_spam_exe basename)
  add_executable(basename ${basename})
  target_link_libraries(${basename} ${basename}.c)
endmacro()
