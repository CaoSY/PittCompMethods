OFILES=add.o parse.o print.o

plus:plus.o libPlus.a
	g++ $< -o $@ -L . -lPlus

libPlus.a:$(OFILES)
	ar rc $@ $?

clean:
	rm -f *.o *.a plus