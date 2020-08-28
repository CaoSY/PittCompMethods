OFILES=add.o parse.o print.o

plus: plus.o libPlus.so
	g++ $< -o $@ -L . -lPlus

libPlus.so : $(OFILES)
	g++ -shared $? -o $@

$(OFILES): %.o : %.cpp
	g++ -fPIC -c $^ -o $@

clean:
	rm -f *.o *.a *.so plus