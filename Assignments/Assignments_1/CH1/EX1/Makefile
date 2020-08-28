EXE_FILE = exp0 exp1 exp2 exp3

all: $(EXE_FILE)

$(EXE_FILE): % : %.cpp
	g++ $^ -o $@

clean:
	rm -f *.o *.a *.so $(EXE_FILE)