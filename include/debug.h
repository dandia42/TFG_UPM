#ifdef DEBUG
#define DOUT std::cout
#else
#define DOUT \
  if (false) std::cout
#endif