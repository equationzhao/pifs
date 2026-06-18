INCLUDES = $(FUSE_CFLAGS) $(EXTRA_CFLAGS)

bin_PROGRAMS=πfs

noinst_HEADERS = pifs_backend.h

__fs_SOURCES=πfs.c piqpr8.c pifs_backend.c

__fs_LDADD = $(FUSE_LIBS) -lm

check_PROGRAMS = test_piqpr8

TESTS = test_piqpr8

test_piqpr8_SOURCES = test_piqpr8.c piqpr8.c pifs_backend.c

test_piqpr8_LDFLAGS = -pthread

test_piqpr8_LDADD = -lm
