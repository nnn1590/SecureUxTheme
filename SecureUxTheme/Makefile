TARGET := x86_64-w64-mingw32
CXX := $(TARGET)-g++
OUTPUT := SecureUxTheme.dll
DEBUG := 0

.PHONY: all clean
all: $(OUTPUT)

$(OUTPUT): avrf.cpp
ifeq ($(DEBUG),1)
	x86_64-w64-mingw32-g++ -shared -o $@ $^ -municode -mwindows -DWIN32 -D_WINDOWS -D_USRDLL -DUNICODE -D_UNICODE -static-libgcc -static-libstdc++ -I. -lntdll -g -D_DEBUG
else
	x86_64-w64-mingw32-g++ -shared -o $@ $^ -municode -mwindows -DWIN32 -D_WINDOWS -D_USRDLL -DUNICODE -D_UNICODE -static-libgcc -static-libstdc++ -I. -lntdll -O2 -DNDEBUG
endif

clean:
	$(RM) -rf $(OUTPUT)
