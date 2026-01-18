CXX = g++
CXXFLAGS = -std=c++11 -pthread
SRC = deadlock.cpp
BIN = bin

all: deadlock producer_consumer simdtest cacheline

$(BIN):
	mkdir -p $(BIN)

deadlock: deadlock.cpp | $(BIN)
	$(CXX) $(CXXFLAGS) -o $(BIN)/deadlock deadlock.cpp

producer_consumer: producer_consumer.cpp | $(BIN)
	$(CXX) $(CXXFLAGS) -o $(BIN)/producer_consumer producer_consumer.cpp

simdtest: simd_test.cpp | $(BIN)
	$(CXX) $(CXXFLAGS) -mavx -o $(BIN)/simdtest simd_test.cpp

cacheline: cacheline.cpp | $(BIN)
	$(CXX) $(CXXFLAGS) -o $(BIN)/cacheline cacheline.cpp

.PHONY: all clean

clean:
	rm -f *.o *.out $(BIN)/*