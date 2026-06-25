CC = g++
flags = -Wall -Wextra -Wundef -Werror -Wuninitialized -Winit-self
lib = libgravitacion.so
exe = prueba


all: $(exe) _gravitacion.so

$(exe): main.o $(lib)
	$(CC) $(flags) main.o -lgravitacion -L. -Wl,-rpath=. -o $(exe)

$(lib): cuerpo.o sim.o
	$(CC) -shared cuerpo.o sim.o -o $(lib)

gravitacion_wrap.cxx: gravitacion.i cuerpo.h sim.h variable2d.h
	swig -c++ -python gravitacion.i

gravitacion_wrap.o: gravitacion_wrap.cxx
	$(CC) $(flags) -fPIC -c gravitacion_wrap.cxx -o gravitacion_wrap.o -I/usr/include/python3.14

_gravitacion.so: gravitacion_wrap.o cuerpo.o sim.o
	$(CC) -shared gravitacion_wrap.o cuerpo.o sim.o -o _gravitacion.so

main.o: main.cpp cuerpo.h sim.h variable2d.h
	$(CC) $(flags) main.cpp -c -o main.o

%.o: %.cpp %.h variable2d.h
	$(CC) $(flags) -fPIC -c $< -o $@

clean:
	rm -rf $(exe) $(lib) *.o gravitacion_wrap.cxx _gravitacion.so gravitacion.py __pycache__