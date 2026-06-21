CC = g++
flags = -Wall -Wextra -Wundef -Werror -Wuninitialized -Winit-self
lib = libgravitacion.so
exe = prueba

$(exe): main.o $(lib)
	$(CC) $(flags) main.o -lgravitacion -L. -Wl,-rpath=. -o $(exe)

$(lib): cuerpo.o sim.o
	$(CC) -shared cuerpo.o sim.o -o $(lib)

main.o: main.cpp 
	$(CC) main.cpp -c -o main.o

%.o: %.cpp
	$(CC) $(flags) -fPIC -c $< -o $@

clean:
	rm -rf $(exe) $(lib) %.o