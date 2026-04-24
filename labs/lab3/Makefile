CXX = g++
CXXFLAGS = -std=c++17 -Wall -g -I. -Igenerated
FLEX = flex
BISON = bison

TARGET = robot
GENDIR = generated

# ----------- СБОРКА -----------

all: $(GENDIR)/$(TARGET)

# ----------- BISON -----------

$(GENDIR)/robot.tab.cpp $(GENDIR)/robot.tab.hpp: robot.ypp ast.hpp | $(GENDIR)
	$(BISON) -d robot.ypp \
		-o $(GENDIR)/robot.tab.cpp \
		--report=all \
		--report-file=$(GENDIR)/robot.report \
		-Wcounterexamples -Wconflicts-rr -Wconflicts-sr

# ----------- FLEX -----------

$(GENDIR)/robot.lex.cpp: robot_lexer.l $(GENDIR)/robot.tab.hpp RobotLexer.h ast.hpp | $(GENDIR)
	$(FLEX) -o $(GENDIR)/robot.lex.cpp robot_lexer.l

# ----------- КОМПИЛЯЦИЯ -----------

$(GENDIR)/$(TARGET): main.cpp \
                     $(GENDIR)/robot.tab.cpp \
                     $(GENDIR)/robot.lex.cpp \
                     RobotLexer.h ast.hpp
	$(CXX) $(CXXFLAGS) -o $@ \
		main.cpp \
		$(GENDIR)/robot.tab.cpp \
		$(GENDIR)/robot.lex.cpp

# ----------- ДИРЕКТОРИЯ -----------

$(GENDIR):
	mkdir -p $(GENDIR)

# ----------- СИМЛИНК -----------

link: $(GENDIR)/$(TARGET)
	ln -sf $(GENDIR)/$(TARGET) $(TARGET)

# ----------- ОЧИСТКА -----------

clean:
	rm -rf $(GENDIR) $(TARGET)

.PHONY: all clean link