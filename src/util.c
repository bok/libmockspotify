#include "libmockspotify.h"

void*
xmalloc(size_t size)
{
  void *ptr = malloc(size);
  memset(ptr, 0, size);
  return ptr;
}

int
htoi(char n)
{
  if (n >= '0' && n <= '9') return n - '0';
  else if (n >= 'a' && n <= 'f') return n - 'a' + 10;
  else if (n >= 'A' && n <= 'F') return n - 'A' + 10;
  else return 0;
}

char
itoh(int n)
{
  char hex[] = { "0123456789abcdef" };
  return hex[n];
}

char*
hextoa(const char *str, int size)
{
  if (size % 2) return NULL;

  char *result = ALLOC_N(char, size / 2);

  int i;
  for (i = 0; i < size; i += 2)
  {
    result[i/2] = (htoi(str[i]) << 4) + htoi(str[i+1]);
  }

  return result;
}

void
atohex(char *dst, const char *src, int size)
{
  int i;
  int p;
  for (i = p = 0; i < size; i += 2, p = i/2)
  {
    dst[i]   = itoh((src[p] >> 4) & 0x0F);
    dst[i+1] = itoh(src[p] & 0xF);
  }
}

void
my_strncpy(char *destination, const char *source, size_t size)
{
  strncpy(destination, source, size - 2);
  destination[size - 1] = '\0';
}