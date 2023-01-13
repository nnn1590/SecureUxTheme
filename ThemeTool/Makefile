TARGET := i686-w64-mingw32
CXX := $(TARGET)-g++
WINDRES := $(TARGET)-windres
CXXFLAGS := -fPIE -municode -DCI_VERSION="\"(unknown)\"" -DUNICODE -D_UNICODE -mwindows -static-libgcc -static-libstdc++
LDFLAGS := $(CXXFLAGS) -ladvapi32 -lcomctl32 -lwtsapi32 -lole32 -loleaut32 -lntdll
WINDRESFLAGS := -DDISABLE_ARM64
OBJ := obj
BIN := bin
SRC := .
RES := res
OUTPUT := $(BIN)/ThemeTool.exe

.PHONY: all
all: $(BIN) $(OBJ) $(OUTPUT)

$(BIN):
	mkdir $@
$(OBJ):
	mkdir $@

$(OUTPUT): $(OBJ)/main.o $(OBJ)/MainDialog.o $(OBJ)/pch.o $(OBJ)/signature.o $(OBJ)/utl.o $(OBJ)/ThemeTool.o
	$(CXX) -o $@ $^ $(CXXFLAGS) $(LDFLAGS)

.SUFFIXS: .cpp .h .rc .o
$(OBJ)/%.o: $(SRC)/%.cpp $(SRC)/%.h
	$(CXX) -c -o $@ $< $(CXXFLAGS)

$(OBJ)/%.o: $(SRC)/%.cpp
	$(CXX) -c -o $@ $< $(CXXFLAGS)

$(OBJ)/%.o: $(SRC)/%.rc
	$(WINDRES) --input-format=rc --output-format=coff -o $@ $< $(WINDRESFLAGS)

.PHONY: clean
clean:
	$(RM) -rf $(BIN)/ $(OBJ)/
