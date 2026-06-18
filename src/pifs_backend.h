#ifndef PIFS_BACKEND_H
#define PIFS_BACKEND_H

struct pifs_backend {
  const char *name;
  unsigned char (*get_byte)(int id);
};

unsigned char get_pi_byte(int id);
unsigned char get_e_byte(int id);

const struct pifs_backend *pifs_default_backend(void);
const struct pifs_backend *pifs_find_backend(const char *name);
const char *pifs_backend_names(void);

#endif
