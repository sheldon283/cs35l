randlibhw.so:
	$(CC) $(CFLAGS) -fPIC -c -o randlibhw.o randlibhw.c
	$(CC) $(CFLAGS) -shared randlibhw.o -o randlibhw.so

randlibsw.so:
	$(CC) $(CFLAGS) -fPIC -c -o randlibsw.o randlibsw.c
	$(CC) $(CFLAGS) -shared randlibsw.o -o randlibsw.so

randmain:
	$(CC) $(CFLAGS) -c -o randcpuid.o randcpuid.c
	$(CC) $(CFLAGS) -c -o randmain.o randmain.c
	$(CC) $(CFLAGS) -ldl -Wl,-rpath=$PWD randmain.o randcpuid.o -o randmain
