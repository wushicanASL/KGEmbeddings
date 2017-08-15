params = -std=c++11 -Ofast -pthread
debugparams = -std=c++11 -g -pthread
bld = build
src = src

EM = EmbeddingModel.o TransE.o TransH.o
EMDebug = EmbeddingModelDebug.o TransEDebug.o TransHDebug.o
EMFile = $(bld)/EmbeddingModel.o $(bld)/TransE.o $(bld)/TransH.o
EMDebugFile = $(bld)/EmbeddingModelDebug.o $(bld)/TransEDebug.o $(bld)/TransHDebug.o
SM = SamplingModel.o unifSampling.o bernSampling.o updateSampling.o
SMDebug = SamplingModelDebug.o unifSamplingDebug.o bernSamplingDebug.o updateSamplingDebug.o
SMFile = $(bld)/SamplingModel.o $(bld)/unifSampling.o $(bld)/bernSampling.o $(bld)/updateSampling.o
SMDebugFile = $(bld)/SamplingModelDebug.o $(bld)/unifSamplingDebug.o $(bld)/bernSamplingDebug.o $(bld)/updateSamplingDebug.o
ALL = $(EM) $(SM) random.o Train.o DataSet.o
ALLDebug = $(EMDebug) $(SMDebug) randomDebug.o TrainDebug.o DataSetDebug.o
ALLFile = $(EMFile) $(SMFile) $(bld)/random.o $(bld)/Train.o $(bld)/DataSet.o
ALLDebugFile = $(EMDebugFile) $(SMDebugFile) $(bld)/randomDebug.o $(bld)/TrainDebug.o $(bld)/DataSetDebug.o

ifeq ($(shell uname), Linux)
ext = out
RM = rm
else
ext = exe
RM = del
endif

test_main: $(ALL) $(src)/test_main.cpp
	g++ $(src)/test_main.cpp $(ALLFile) -o test_main.$(ext) $(params)

test_mainDebug: $(ALLDebug) $(src)/test_main.cpp
	g++ $(src)/test_main.cpp $(ALLDebugFile) -o test_mainDebug.$(ext) $(debugparams)

getRbox: DataSet.o $(src)/getRbox.cpp
	g++ $(src)/getRbox.cpp $(bld)/DataSet.o -o getRbox.$(ext) $(params)

getRboxDebug: DataSetDebug.o $(src)/getRbox.cpp
	g++ $(src)/getRbox.cpp $(bld)/DataSetDebug.o -o getRboxDebug.$(ext) $(debugparams)

Train.o: $(src)/Train.cpp
	g++ $(src)/Train.cpp -c -o $(bld)/Train.o $(params)

TrainDebug.o: $(src)/Train.cpp
	g++ $(src)/Train.cpp -c -o $(bld)/TrainDebug.o $(debugparams)

random.o: $(src)/random.cpp
	g++ $(src)/random.cpp -c -o $(bld)/random.o $(params)

randomDebug.o: $(src)/random.cpp
	g++ $(src)/random.cpp -c -o $(bld)/randomDebug.o $(debugparams)

DataSet.o: $(src)/DataSet.cpp
	g++ $(src)/DataSet.cpp -c -o $(bld)/DataSet.o $(params)

DataSetDebug.o: $(src)/DataSet.cpp
	g++ $(src)/DataSet.cpp -c -o $(bld)/DataSetDebug.o $(debugparams)

EmbeddingModel.o: $(src)/EmbeddingModel.cpp
	g++ $(src)/EmbeddingModel.cpp -c -o $(bld)/EmbeddingModel.o $(params)

EmbeddingModelDebug.o: $(src)/EmbeddingModel.cpp
	g++ $(src)/EmbeddingModel.cpp -c -o $(bld)/EmbeddingModelDebug.o $(debugparams)

SamplingModel.o: $(src)/SamplingModel.cpp
	g++ $(src)/SamplingModel.cpp -c -o $(bld)/SamplingModel.o $(params)

SamplingModelDebug.o: $(src)/SamplingModel.cpp
	g++ $(src)/SamplingModel.cpp -c -o $(bld)/SamplingModelDebug.o $(debugparams)

TransE.o: $(src)/TransE.cpp
	g++ $(src)/TransE.cpp -c -o $(bld)/TransE.o $(params)

TransEDebug.o: $(src)/TransE.cpp
	g++ $(src)/TransE.cpp -c -o $(bld)/TransEDebug.o $(debugparams)

TransH.o: $(src)/TransH.cpp
	g++ $(src)/TransH.cpp -c -o $(bld)/TransH.o $(params)

TransHDebug.o: $(src)/TransH.cpp
	g++ $(src)/TransH.cpp -c -o $(bld)/TransHDebug.o $(debugparams)

unifSampling.o: $(src)/unifSampling.cpp
	g++ $(src)/unifSampling.cpp -c -o $(bld)/unifSampling.o $(params)

unifSamplingDebug.o: $(src)/unifSampling.cpp
	g++ $(src)/unifSampling.cpp -c -o $(bld)/unifSamplingDebug.o $(debugparams)

bernSampling.o: $(src)/bernSampling.cpp
	g++ $(src)/bernSampling.cpp -c -o $(bld)/bernSampling.o $(params)

bernSamplingDebug.o: $(src)/bernSampling.cpp
	g++ $(src)/bernSampling.cpp -c -o $(bld)/bernSamplingDebug.o $(debugparams)

updateSampling.o: $(src)/updateSampling.cpp
	g++ $(src)/updateSampling.cpp -c -o $(bld)/updateSampling.o $(params)

updateSamplingDebug.o: $(src)/updateSampling.cpp
	g++ $(src)/updateSampling.cpp -c -o $(bld)/updateSamplingDebug.o $(debugparams)

.PHONY: clean
clean:
	-$(RM) *.exe *.out $(bld)/*