SRCS	:= $(wildcard *.cpp)
BINS	:= $(SRCS:%.cpp=%.out)

all: $(BINS)

%.out: %.cpp
	@echo "[LD] $@"
	@$(CXX) -g $< -std=c++17 -o $@

clean:
	rm -rf *.out