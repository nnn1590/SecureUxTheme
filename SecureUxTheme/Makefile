TARGET := x86_64-w64-mingw32
CXX := $(TARGET)-g++
OUTPUT := SecureUxTheme.dll
CXXFLAGS := -municode -mwindows -DWIN32 -D_WINDOWS -D_USRDLL -DUNICODE -D_UNICODE -static-libgcc -static-libstdc++ -I. -lntdll
DEBUG := 0
DEBUGCXXFLAGS := -g -D_DEBUG
RELEASECXXFLAGS := -O2 -DNDEBUG
ifeq ($(DEBUG),1)
	CXXFLAGS := $(CXXFLAGS) $(DEBUGCXXFLAGS)
else
	CXXFLAGS := $(CXXFLAGS) $(RELEASECXXFLAGS)
endif

.PHONY: all clean
all: $(OUTPUT)

$(OUTPUT): avrf.cpp
	x86_64-w64-mingw32-g++ -shared -o $@ $^ $(CXXFLAGS)

clean:
	$(RM) -rf $(OUTPUT)
