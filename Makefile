INCFLAGS = -I/usr/local/include/ -I./src/

CPP = g++ -std=c++11
CPPFLAGS = -g -O0 $(INCFLAGS) -Wall -Wno-strict-aliasing # -fopenmp # 注意添加链接库
LINKERFLAGS = -lrocksdb -lz -lpthread -ldl
DEBUGFLAGS = -g -ggdb $(INCFLAGS)
HEADERS=$(shell find . -name '*.hpp')

all: db apps

apps : apps/importdb apps/testic1

apps/% : apps/%.cpp $(HEADERS)
	@mkdir -p bin/$(@D)
	$(CPP) $(CPPFLAGS) -Iapp/ $@.cpp -o bin/$@ $(LINKERFLAGS)

db: src/db.cpp
	mkdir -p bin/
	$(CXX) $(CXXFLAGS) -o bin/$@ $< 

run: db
	./bin/db

clean:
	rm -f *.o
	rm -rf ./bin
	@rm -rf bin/*
