CC = acom
CFLAGS = -I.
RAY2_FILES = D3d_matrix.c vecops.c patterns.c objs.c ray_tracing2d.c 
RAY3_FILES = D3d_matrix.c vecops.c patterns.c objs.c ray_tracing3d.c 
TET_FILES = D3d_matrix.c vecops.c patterns.c objs.c tetrahedron.c
PAR_FILES = D3d_matrix.c vecops.c patterns.c objs.c parametric_test.c

ray_tracing2d: $(RAY2_FILES)
	$(CC) $(CFLAGS) $(RAY2_FILES)

ray_tracing3d: $(RAY3_FILES)
	$(CC) $(CFLAGS) $(RAY3_FILES)

tetrahedron: $(TET_FILES)
	$(CC) $(CFLAGS) $(TET_FILES)

parametric_test: $(PAR_FILES)
	$(CC) $(CFLAGS) $(PAR_FILES)

run: a.out
	./a.out
