#include "pifs_backend.h"

#include <string.h>
#include <strings.h>

struct pifs_backend_alias {
  const char *name;
  const struct pifs_backend *backend;
};

static const struct pifs_backend pi_backend = { "pi", get_pi_byte };
static const struct pifs_backend e_backend = { "e", get_e_byte };

static const struct pifs_backend_alias pifs_backend_aliases[] = {
  { "pi", &pi_backend },
  { "π", &pi_backend },
  { "e", &e_backend },
  { NULL, NULL },
};

const struct pifs_backend *pifs_default_backend(void)
{
  return &pi_backend;
}

const struct pifs_backend *pifs_find_backend(const char *name)
{
  if (!name || !*name) {
    return pifs_default_backend();
  }

  for (int i = 0; pifs_backend_aliases[i].name; i++) {
    if (strcasecmp(name, pifs_backend_aliases[i].name) == 0 ||
        strcmp(name, pifs_backend_aliases[i].name) == 0) {
      return pifs_backend_aliases[i].backend;
    }
  }

  return NULL;
}

const char *pifs_backend_names(void)
{
  return "pi, π, e";
}
