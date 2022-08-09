#include <assert.h>
#include <stdlib.h>

#include <spam.h>

int
spam_matrix_new(SPAMatrix *m_ptr, size_t m, size_t n) {

  assert(m_ptr);
  assert(m > 0 && n > 0);

  *m_ptr = malloc(sizeof(**m_ptr) + sizeof(double) * (m * n - 1));

  if (!(*m_ptr))
    return SPAM_MEM_ERROR;

  return SPAM_NO_ERROR;
}

void
spam_matrix_free(SPAMatrix *m_ptr) {

  if (!m_ptr || !(*m_ptr))
    return;

  free(*m_ptr);
  *m_ptr = NULL;
}
