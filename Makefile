
# http://www.flintlib.org/
# https://www.mpfr.org/
# http://www.gmplib.org/
# alternative to gmp: http://www.mpir.org/

.PHONY: build
build: m

.PHONY: run
run:
	LD_LIBRARY_PATH=arb ./m

o = grx.o

grx.o: grx.c gr.h

m: m.c $(o)
	gcc -Iarb $< -Larb -larb -lflint $(o) -lX11 -o $@

.c.o:
	gcc -c $< -o $@

.PHONY: arb
arb:
	apt-get install libflint-dev libgmp-dev libmpfr-dev
	cd arb && ./configure
	cd arb && make -j4
	# apt-get install python-sphinx
	pip3 install -U sphinx
	cd arb/doc && make -j4
