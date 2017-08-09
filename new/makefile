params = -std=c++11 -Ofast -pthread
debugparams = -std=c++11 -g -pthread
bld = build
src = src

ifeq ($(shell uname), Linux)
ext = out
RM = rm
else
ext = exe
RM = del
endif

test_main: Train.o DataSet.o TransE.o unifSampling.o bernSampling.o EmbeddingModel.o SamplingModel.o random.o $(src)/test_main.cpp
	g++ $(src)/test_main.cpp $(bld)/DataSet.o $(bld)/Train.o $(bld)/unifSampling.o $(bld)/bernSampling.o $(bld)/TransE.o $(bld)/EmbeddingModel.o $(bld)/SamplingModel.o $(bld)/random.o -o test_main.$(ext) $(params)

test_mainDebug: TrainDebug.o DataSetDebug.o TransEDebug.o unifSamplingDebug.o bernSamplingDebug.o EmbeddingModelDebug.o SamplingModelDebug.o randomDebug.o $(src)/test_main.cpp
	g++ $(src)/test_main.cpp $(bld)/DataSetDebug.o $(bld)/TrainDebug.o $(bld)/unifSamplingDebug.o $(bld)/bernSamplingDebug.o $(bld)/TransEDebug.o $(bld)/EmbeddingModelDebug.o $(bld)/SamplingModelDebug.o $(bld)/randomDebug.o -o test_mainDebug.$(ext) $(debugparams)

getRbox: DataSet.o $(src)/getRbox.cpp
	g++ $(src)/getRbox.cpp $(bld)/DataSet.o -o getRbox.$(ext) $(params)

getRboxDebug: DataSetDebug.o $(src)/getRbox.cpp
	g++ $(src)/getRbox.cpp $(bld)/DataSetDebug.o -o getRboxDebug.$(ext) $(debugparams)

Train.o: $(src)/Train.h $(src)/Train.cpp
	g++ $(src)/Train.cpp -c -o $(bld)/Train.o $(params)

TrainDebug.o: $(src)/Train.h $(src)/Train.cpp
	g++ $(src)/Train.cpp -c -o $(bld)/TrainDebug.o $(debugparams)

random.o: $(src)/random.h $(src)/random.cpp
	g++ $(src)/random.cpp -c -o $(bld)/random.o $(params)

randomDebug.o: $(src)/random.h $(src)/random.cpp
	g++ $(src)/random.cpp -c -o $(bld)/randomDebug.o $(debugparams)

DataSet.o: $(src)/DataSet.cpp $(src)/DataSet.h $(src)/Triple.h
	g++ $(src)/DataSet.cpp -c -o $(bld)/DataSet.o $(params)

DataSetDebug.o: $(src)/DataSet.cpp $(src)/DataSet.h $(src)/Triple.h
	g++ $(src)/DataSet.cpp -c -o $(bld)/DataSetDebug.o $(debugparams)

EmbeddingModel.o: $(src)/EmbeddingModel.cpp $(src)/EmbeddingModel.h $(src)/random.h $(src)/Triple.h
	g++ $(src)/EmbeddingModel.cpp -c -o $(bld)/EmbeddingModel.o $(params)

EmbeddingModelDebug.o: $(src)/EmbeddingModel.cpp $(src)/EmbeddingModel.h $(src)/random.h $(src)/Triple.h
	g++ $(src)/EmbeddingModel.cpp -c -o $(bld)/EmbeddingModelDebug.o $(debugparams)

SamplingModel.o: $(src)/random.h $(src)/Triple.h $(src)/SamplingModel.h $(src)/SamplingModel.cpp
	g++ $(src)/SamplingModel.cpp -c -o $(bld)/SamplingModel.o $(params)

SamplingModelDebug.o: $(src)/random.h $(src)/Triple.h $(src)/SamplingModel.h $(src)/SamplingModel.cpp
	g++ $(src)/SamplingModel.cpp -c -o $(bld)/SamplingModelDebug.o $(debugparams)

TransE.o: $(src)/Triple.h $(src)/TransE.h $(src)/TransE.cpp
	g++ $(src)/TransE.cpp -c -o $(bld)/TransE.o $(params)

TransEDebug.o: $(src)/Triple.h $(src)/TransE.h $(src)/TransE.cpp
	g++ $(src)/TransE.cpp -c -o $(bld)/TransEDebug.o $(debugparams)

unifSampling.o: $(src)/Triple.h $(src)/random.h $(src)/unifSampling.h $(src)/unifSampling.cpp
	g++ $(src)/unifSampling.cpp -c -o $(bld)/unifSampling.o $(params)

unifSamplingDebug.o: $(src)/Triple.h $(src)/random.h $(src)/unifSampling.h $(src)/unifSampling.cpp
	g++ $(src)/unifSampling.cpp -c -o $(bld)/unifSamplingDebug.o $(debugparams)

bernSampling.o: $(src)/Triple.h $(src)/random.h $(src)/bernSampling.h $(src)/bernSampling.cpp
	g++ $(src)/bernSampling.cpp -c -o $(bld)/bernSampling.o $(params)

bernSamplingDebug.o: $(src)/Triple.h $(src)/random.h $(src)/bernSampling.h $(src)/bernSampling.cpp
	g++ $(src)/bernSampling.cpp -c -o $(bld)/bernSamplingDebug.o $(debugparams)

.PHONY: clean
clean:
	-$(RM) *.exe *.out $(bld)/*