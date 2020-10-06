CXXFLAGS = -g -O0 -I..
LDFLAGS  = -L../local/lib -lMueller
../local/bin/client:client.cpp ../local/lib/libMueller.a
	mkdir -p ../local/bin
	$(CXX) $(CXXFLAGS) $< $(LDFLAGS) -o $@ 

../local/lib/libMueller.a:Mueller.o
	mkdir -p ../local/lib
	ar rc $@ $<
	ranlib $@

clean:
	rm -f ../local/bin/client Mueller.o ../local/lib/libMueller.a *~
