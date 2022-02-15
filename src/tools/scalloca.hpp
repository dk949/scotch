#ifndef ALLOCA_HPP
#define ALLOCA_HPP

#ifdef SCOTCH_CAN_HAVE_ALLOCA
#define scalloca(sz) alloca(sz)
#define scfreea(ptr)
#else
#define scalloca(sz) malloc(sz)
#define scfreea(ptr) free(ptr)
#endif

#endif  // ALLOCA_HPP
