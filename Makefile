CXX      := g++
CXXFLAGS := -std=c++17 -Wall -Wextra -Iinclude
SRC      := src/main.cpp src/Account.cpp src/Bank.cpp
TARGET   := bank

$(TARGET): $(SRC) $(wildcard include/*.h)
	$(CXX) $(CXXFLAGS) $(SRC) -o $(TARGET)

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET) $(TARGET).exe
