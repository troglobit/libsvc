#pragma once

#if (__GNUC__ >= 4 && __GNUC_MINOR__ >=3) || (__GNUC__ >= 5) || defined(__APPLE__) || defined(__native_client__)

typedef struct atomic {
  int v;
} atomic_t;


static inline void
atomic_inc(atomic_t *a)
{
  __sync_add_and_fetch(&a->v, 1);
}

static inline void
atomic_add(atomic_t *a, int v)
{
  __sync_add_and_fetch(&a->v, v);
}

static inline int __attribute__((warn_unused_result))
atomic_add_and_fetch(atomic_t *a, int v)
{
  return __sync_add_and_fetch(&a->v, v);
}

static inline int
atomic_dec(atomic_t *a)
{
  return __sync_add_and_fetch(&a->v, -1);
}

static inline int
atomic_get(const atomic_t *a)
{
  return __atomic_load_n(&a->v, __ATOMIC_SEQ_CST);
}

static inline void
atomic_set(atomic_t *a, int v)
{
  __atomic_store_n(&a->v, v, __ATOMIC_SEQ_CST);
}

static inline int
atomic_get_and_set(atomic_t *a, int v)
{
  return __sync_lock_test_and_set(&a->v, v);
}


static inline void
atomic_max(atomic_t *a, int v)
{
  while(1) {
    const int current = atomic_get(a);
    const int newval = v > current ? v : current;
    if(__sync_bool_compare_and_swap(&a->v, current, newval))
      break;
  }
}



#else
#error Missing atomic ops
#endif

