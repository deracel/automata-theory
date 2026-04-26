CXX = g++
CXXFLAGS = -std=c++17 -Wall -g -I. -Igenerated
FLEX = flex
BISON = bison

TARGET = robot
GENDIR = generated
VISUALIZER = visualizer/robot_viz

all: $(GENDIR)/$(TARGET) $(VISUALIZER)

# C++ парсер
$(GENDIR)/robot.tab.cpp $(GENDIR)/robot.tab.hpp: robot.ypp ast.hpp | $(GENDIR)
	$(BISON) -d robot.ypp -o $(GENDIR)/robot.tab.cpp

$(GENDIR)/robot.lex.cpp: robot_lexer.l $(GENDIR)/robot.tab.hpp RobotLexer.h ast.hpp | $(GENDIR)
	$(FLEX) -o $(GENDIR)/robot.lex.cpp robot_lexer.l

$(GENDIR)/$(TARGET): main.cpp interpreter.cpp $(GENDIR)/robot.tab.cpp $(GENDIR)/robot.lex.cpp RobotLexer.h ast.hpp
	$(CXX) $(CXXFLAGS) -o $@ main.cpp interpreter.cpp $(GENDIR)/robot.tab.cpp $(GENDIR)/robot.lex.cpp

# Go визуализатор
$(VISUALIZER): visualizer/main.go
	cd visualizer && go build -o robot_viz main.go

$(GENDIR):
	mkdir -p $(GENDIR)

clean:
	rm -rf $(GENDIR) $(VISUALIZER)

run: all
	./$(GENDIR)/$(TARGET) program.xml --map map.txt --vis $(VISUALIZER)

.PHONY: all clean run