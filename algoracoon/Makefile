CXX      := g++
CXXFLAGS := -std=c++17 -Wall -Wextra -pedantic -I.
TARGET   := test_algoracoon.exe

all: $(TARGET)

$(TARGET): tests/main.cpp lib/*.hpp
	$(CXX) $(CXXFLAGS) tests/main.cpp -o $(TARGET)

run: $(TARGET)
	$(TARGET)

clean:
	del /f /q $(TARGET) 2>nul || true

.PHONY: all run clean
