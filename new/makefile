params = -std=c++11 -Ofast -pthread
debugparams = -std=c++11 -g -pthread
bld = build
src = src

getRbox: DataSet.o $(src)/getRbox.cpp
	g++ $(src)/getRbox.cpp $(bld)/DataSet.o -o getRbox $(params)

getRboxDebug: DataSet.o $(src)/getRbox.cpp
	g++ $(src)/getRbox.cpp $(bld)/DataSetDebug.o -o getRboxDebug $(debugparams)

DataSet.o: $(src)/DataSet.cpp $(src)/DataSet.h $(src)/Triple.h
	g++ $(src)/DataSet.cpp -c -o $(bld)/DataSet.o $(params)

DataSetDebug.o: $(src)/DataSet.cpp $(src)/DataSet.h $(src)/Triple.h
	g++ $(src)/DataSet.cpp -c -o $(bld)/DataSetDebug.o $debugparams)

.PHONY: clean
clean:
	-rm *Debug getRbox $(bld)/*