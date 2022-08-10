#include <assert.h>
#include <errno.h>
#include <stdbool.h>
#include <stdlib.h>

#include <spam.h>

int
spam_mat_new(SPAMatrix *m_ptr, size_t m, size_t n) {

  assert(m_ptr);
  assert(m > 0 && n > 0);

  errno  = 0;
  *m_ptr = malloc(sizeof(**m_ptr) + sizeof(double) * (m * n - 1));
  if (!(*m_ptr))
    return SPAM_MEM_ERROR;

  (*m_ptr)->n_rows = m;
  (*m_ptr)->n_cols = n;

  return SPAM_NO_ERROR;
}

int
spam_mat_new_zeros(SPAMatrix *m_ptr, size_t m, size_t n) {

  assert(m_ptr);
  assert(m > 0 && n > 0);

  int new_status = spam_mat_new(m_ptr, m, n);
  if (new_status == SPAM_MEM_ERROR)
    return new_status;

  for (size_t i = 0; i < m * n; i++)
    (*m_ptr)->elements[i] = 0;

  return SPAM_NO_ERROR;
}

int
spam_mat_new_ones(SPAMatrix *m_ptr, size_t m, size_t n) {

  assert(m_ptr);
  assert(m > 0 && n > 0);

  int new_status = spam_mat_new(m_ptr, m, n);
  if (new_status == SPAM_MEM_ERROR)
    return new_status;

  for (size_t i = 0; i < m * n; i++)
    (*m_ptr)->elements[i] = 1;

  return SPAM_NO_ERROR;
}

int
spam_mat_new_memcpy(SPAMatrix *m_ptr, const double *src, size_t m, size_t n) {

  assert(m_ptr);
  assert(src);
  assert(m > 0 && n > 0);

  int new_status = spam_mat_new(m_ptr, m, n);
  if (new_status == SPAM_MEM_ERROR) {
    return new_status;
  }

  for (size_t i = 0; i < m * n; i++)
    (*m_ptr)->elements[i] = src[i];

  return SPAM_NO_ERROR;
}

int
spam_mat_new_copy(SPAMatrix *m_ptr, SPAMatrix a) {

  assert(m_ptr);
  assert(a);

  int new_status =
      spam_mat_new_memcpy(m_ptr, a->elements, a->n_rows, a->n_cols);
  if (new_status == SPAM_MEM_ERROR)
    return new_status;

  return SPAM_NO_ERROR;
}

int
spam_mat_new_like(SPAMatrix *m_ptr, SPAMatrix a) {

  assert(m_ptr);
  assert(a);

  int new_status = spam_mat_new(m_ptr, a->n_rows, a->n_cols);
  if (new_status == SPAM_MEM_ERROR)
    return new_status;

  return SPAM_NO_ERROR;
}

void
spam_mat_free(SPAMatrix *m_ptr) {

  if (!m_ptr || !(*m_ptr))
    return;

  free(*m_ptr);
  *m_ptr = NULL;
}

bool
spam_mat_eq(SPAMatrix a, SPAMatrix b) {

  // return false if either a or b is NULL
  if (!a || !b)
    return false;

  if (a == b)
    return true;

  if ((a->n_rows != b->n_rows) || (a->n_cols != b->n_cols))
    return false;

  for (size_t i = 0; i < a->n_rows * a->n_cols; ++i) {
    if (a->elements[i] != b->elements[i])
      return false;
  }

  return true;
}

SPAMatrixSize
spam_mat_size(SPAMatrix a) {

  assert(a);

  SPAMatrixSize size = {.n_rows = a->n_rows, .n_cols = a->n_cols};

  return size;
}
