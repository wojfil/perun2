WORKDIR = %cd%

CC = gcc.exe
CXX = g++.exe
AR = ar.exe
LD = g++.exe
WINDRES = windres.exe

INC = 
CFLAGS = -Wall -std=c17 -std=c++17 -fexceptions
RESINC = 
LIBDIR = 
LIB = 
LDFLAGS = -static-libstdc++ -static-libgcc -static -municode -unicode -lole32 -loleaut32

INC_DEBUG = $(INC)
CFLAGS_DEBUG = $(CFLAGS) -std=c17 -g -std=c++17
RESINC_DEBUG = $(RESINC)
RCFLAGS_DEBUG = $(RCFLAGS)
LIBDIR_DEBUG = $(LIBDIR)
LIB_DEBUG = $(LIB)
LDFLAGS_DEBUG = $(LDFLAGS) -static-libstdc++ -static-libgcc -static -municode -unicode -lole32 -loleaut32
OBJDIR_DEBUG = obj\\Debug
DEP_DEBUG = 
OUT_DEBUG = bin\\Debug\\Uroboros.exe

INC_RELEASE = $(INC)
CFLAGS_RELEASE = $(CFLAGS) -fomit-frame-pointer -std=c17 -std=c++17
RESINC_RELEASE = $(RESINC)
RCFLAGS_RELEASE = $(RCFLAGS)
LIBDIR_RELEASE = $(LIBDIR)
LIB_RELEASE = $(LIB)
LDFLAGS_RELEASE = $(LDFLAGS) -s -static-libstdc++ -static-libgcc -static -municode -unicode -lole32 -loleaut32
OBJDIR_RELEASE = obj\\Release
DEP_RELEASE = 
OUT_RELEASE = bin\\Release\\Uroboros.exe

OBJ_DEBUG = $(OBJDIR_DEBUG)\\datatype\\parse\\parse-definition.o $(OBJDIR_DEBUG)\\datatype\\parse\\parse-generic.o $(OBJDIR_DEBUG)\\datatype\\parse\\parse-list.o $(OBJDIR_DEBUG)\\datatype\\parse\\parse-number.o $(OBJDIR_DEBUG)\\datatype\\parse\\parse-numlist.o $(OBJDIR_DEBUG)\\datatype\\parse\\parse-period.o $(OBJDIR_DEBUG)\\datatype\\parse\\parse-string.o $(OBJDIR_DEBUG)\\datatype\\parse\\parse-time.o $(OBJDIR_DEBUG)\\datatype\\parse\\parse-timlist.o $(OBJDIR_DEBUG)\\datatype\\generator\\gen-period.o $(OBJDIR_DEBUG)\\datatype\\generator\\gen-string.o $(OBJDIR_DEBUG)\\datatype\\generator\\gen-time.o $(OBJDIR_DEBUG)\\datatype\\generator\\gen-timlist.o $(OBJDIR_DEBUG)\\datatype\\generator\\like.o $(OBJDIR_DEBUG)\\datatype\\number.o $(OBJDIR_DEBUG)\\datatype\\order.o $(OBJDIR_DEBUG)\\datatype\\parse-gen.o $(OBJDIR_DEBUG)\\datatype\\parse\\parse-bool.o $(OBJDIR_DEBUG)\\token.o $(OBJDIR_DEBUG)\\tokens.o $(OBJDIR_DEBUG)\\uroboros.o $(OBJDIR_DEBUG)\\util.o $(OBJDIR_DEBUG)\\var\\var-group.o $(OBJDIR_DEBUG)\\var\\var-inner.o $(OBJDIR_DEBUG)\\var\\var-runtime.o $(OBJDIR_DEBUG)\\var\\var.o $(OBJDIR_DEBUG)\\wndres.o $(OBJDIR_DEBUG)\\datatype\\period.o $(OBJDIR_DEBUG)\\datatype\\primitives.o $(OBJDIR_DEBUG)\\datatype\\time.o $(OBJDIR_DEBUG)\\exception.o $(OBJDIR_DEBUG)\\hash.o $(OBJDIR_DEBUG)\\keyword.o $(OBJDIR_DEBUG)\\lexer.o $(OBJDIR_DEBUG)\\main.o $(OBJDIR_DEBUG)\\os.o $(OBJDIR_DEBUG)\\print.o $(OBJDIR_DEBUG)\\command\\com-parse-kw.o $(OBJDIR_DEBUG)\\command\\com-parse-unit.o $(OBJDIR_DEBUG)\\command\\com-parse.o $(OBJDIR_DEBUG)\\command\\com-struct.o $(OBJDIR_DEBUG)\\command\\com-var.o $(OBJDIR_DEBUG)\\command\\com.o $(OBJDIR_DEBUG)\\datatype\\cast.o $(OBJDIR_DEBUG)\\datatype\\def-var.o $(OBJDIR_DEBUG)\\attr-memory.o $(OBJDIR_DEBUG)\\attribute.o $(OBJDIR_DEBUG)\\brackets.o $(OBJDIR_DEBUG)\\command\\aggregate.o $(OBJDIR_DEBUG)\\command\\com-arg.o $(OBJDIR_DEBUG)\\command\\com-condition.o $(OBJDIR_DEBUG)\\command\\com-core-aggr.o $(OBJDIR_DEBUG)\\command\\com-core.o $(OBJDIR_DEBUG)\\command\\com-create.o $(OBJDIR_DEBUG)\\command\\com-misc.o $(OBJDIR_DEBUG)\\datatype\\function\\func-time.o $(OBJDIR_DEBUG)\\datatype\\generator.o $(OBJDIR_DEBUG)\\datatype\\generator\\gen-bool-compare.o $(OBJDIR_DEBUG)\\datatype\\generator\\gen-bool.o $(OBJDIR_DEBUG)\\datatype\\generator\\gen-definition.o $(OBJDIR_DEBUG)\\datatype\\generator\\gen-list.o $(OBJDIR_DEBUG)\\datatype\\generator\\gen-number.o $(OBJDIR_DEBUG)\\datatype\\generator\\gen-numlist.o $(OBJDIR_DEBUG)\\datatype\\generator\\gen-os.o $(OBJDIR_DEBUG)\\datatype\\definition.o $(OBJDIR_DEBUG)\\datatype\\function.o $(OBJDIR_DEBUG)\\datatype\\function\\func-aggr.o $(OBJDIR_DEBUG)\\datatype\\function\\func-attr.o $(OBJDIR_DEBUG)\\datatype\\function\\func-bool.o $(OBJDIR_DEBUG)\\datatype\\function\\func-list.o $(OBJDIR_DEBUG)\\datatype\\function\\func-number.o $(OBJDIR_DEBUG)\\datatype\\function\\func-string.o

OBJ_RELEASE = $(OBJDIR_RELEASE)\\datatype\\parse\\parse-definition.o $(OBJDIR_RELEASE)\\datatype\\parse\\parse-generic.o $(OBJDIR_RELEASE)\\datatype\\parse\\parse-list.o $(OBJDIR_RELEASE)\\datatype\\parse\\parse-number.o $(OBJDIR_RELEASE)\\datatype\\parse\\parse-numlist.o $(OBJDIR_RELEASE)\\datatype\\parse\\parse-period.o $(OBJDIR_RELEASE)\\datatype\\parse\\parse-string.o $(OBJDIR_RELEASE)\\datatype\\parse\\parse-time.o $(OBJDIR_RELEASE)\\datatype\\parse\\parse-timlist.o $(OBJDIR_RELEASE)\\datatype\\generator\\gen-period.o $(OBJDIR_RELEASE)\\datatype\\generator\\gen-string.o $(OBJDIR_RELEASE)\\datatype\\generator\\gen-time.o $(OBJDIR_RELEASE)\\datatype\\generator\\gen-timlist.o $(OBJDIR_RELEASE)\\datatype\\generator\\like.o $(OBJDIR_RELEASE)\\datatype\\number.o $(OBJDIR_RELEASE)\\datatype\\order.o $(OBJDIR_RELEASE)\\datatype\\parse-gen.o $(OBJDIR_RELEASE)\\datatype\\parse\\parse-bool.o $(OBJDIR_RELEASE)\\token.o $(OBJDIR_RELEASE)\\tokens.o $(OBJDIR_RELEASE)\\uroboros.o $(OBJDIR_RELEASE)\\util.o $(OBJDIR_RELEASE)\\var\\var-group.o $(OBJDIR_RELEASE)\\var\\var-inner.o $(OBJDIR_RELEASE)\\var\\var-runtime.o $(OBJDIR_RELEASE)\\var\\var.o $(OBJDIR_RELEASE)\\wndres.o $(OBJDIR_RELEASE)\\datatype\\period.o $(OBJDIR_RELEASE)\\datatype\\primitives.o $(OBJDIR_RELEASE)\\datatype\\time.o $(OBJDIR_RELEASE)\\exception.o $(OBJDIR_RELEASE)\\hash.o $(OBJDIR_RELEASE)\\keyword.o $(OBJDIR_RELEASE)\\lexer.o $(OBJDIR_RELEASE)\\main.o $(OBJDIR_RELEASE)\\os.o $(OBJDIR_RELEASE)\\print.o $(OBJDIR_RELEASE)\\command\\com-parse-kw.o $(OBJDIR_RELEASE)\\command\\com-parse-unit.o $(OBJDIR_RELEASE)\\command\\com-parse.o $(OBJDIR_RELEASE)\\command\\com-struct.o $(OBJDIR_RELEASE)\\command\\com-var.o $(OBJDIR_RELEASE)\\command\\com.o $(OBJDIR_RELEASE)\\datatype\\cast.o $(OBJDIR_RELEASE)\\datatype\\def-var.o $(OBJDIR_RELEASE)\\attr-memory.o $(OBJDIR_RELEASE)\\attribute.o $(OBJDIR_RELEASE)\\brackets.o $(OBJDIR_RELEASE)\\command\\aggregate.o $(OBJDIR_RELEASE)\\command\\com-arg.o $(OBJDIR_RELEASE)\\command\\com-condition.o $(OBJDIR_RELEASE)\\command\\com-core-aggr.o $(OBJDIR_RELEASE)\\command\\com-core.o $(OBJDIR_RELEASE)\\command\\com-create.o $(OBJDIR_RELEASE)\\command\\com-misc.o $(OBJDIR_RELEASE)\\datatype\\function\\func-time.o $(OBJDIR_RELEASE)\\datatype\\generator.o $(OBJDIR_RELEASE)\\datatype\\generator\\gen-bool-compare.o $(OBJDIR_RELEASE)\\datatype\\generator\\gen-bool.o $(OBJDIR_RELEASE)\\datatype\\generator\\gen-definition.o $(OBJDIR_RELEASE)\\datatype\\generator\\gen-list.o $(OBJDIR_RELEASE)\\datatype\\generator\\gen-number.o $(OBJDIR_RELEASE)\\datatype\\generator\\gen-numlist.o $(OBJDIR_RELEASE)\\datatype\\generator\\gen-os.o $(OBJDIR_RELEASE)\\datatype\\definition.o $(OBJDIR_RELEASE)\\datatype\\function.o $(OBJDIR_RELEASE)\\datatype\\function\\func-aggr.o $(OBJDIR_RELEASE)\\datatype\\function\\func-attr.o $(OBJDIR_RELEASE)\\datatype\\function\\func-bool.o $(OBJDIR_RELEASE)\\datatype\\function\\func-list.o $(OBJDIR_RELEASE)\\datatype\\function\\func-number.o $(OBJDIR_RELEASE)\\datatype\\function\\func-string.o

all: debug release

clean: clean_debug clean_release

before_debug: 
	cmd /c if not exist bin\\Debug md bin\\Debug
	cmd /c if not exist $(OBJDIR_DEBUG)\\datatype\\parse md $(OBJDIR_DEBUG)\\datatype\\parse
	cmd /c if not exist $(OBJDIR_DEBUG)\\datatype\\generator md $(OBJDIR_DEBUG)\\datatype\\generator
	cmd /c if not exist $(OBJDIR_DEBUG)\\datatype md $(OBJDIR_DEBUG)\\datatype
	cmd /c if not exist $(OBJDIR_DEBUG) md $(OBJDIR_DEBUG)
	cmd /c if not exist $(OBJDIR_DEBUG)\\var md $(OBJDIR_DEBUG)\\var
	cmd /c if not exist $(OBJDIR_DEBUG)\\command md $(OBJDIR_DEBUG)\\command
	cmd /c if not exist $(OBJDIR_DEBUG)\\datatype\\function md $(OBJDIR_DEBUG)\\datatype\\function

after_debug: 

debug: before_debug out_debug after_debug

out_debug: before_debug $(OBJ_DEBUG) $(DEP_DEBUG)
	$(LD) $(LIBDIR_DEBUG) -o $(OUT_DEBUG) $(OBJ_DEBUG)  $(LDFLAGS_DEBUG) $(LIB_DEBUG)

$(OBJDIR_DEBUG)\\datatype\\parse\\parse-definition.o: datatype\\parse\\parse-definition.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c datatype\\parse\\parse-definition.cpp -o $(OBJDIR_DEBUG)\\datatype\\parse\\parse-definition.o

$(OBJDIR_DEBUG)\\datatype\\parse\\parse-generic.o: datatype\\parse\\parse-generic.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c datatype\\parse\\parse-generic.cpp -o $(OBJDIR_DEBUG)\\datatype\\parse\\parse-generic.o

$(OBJDIR_DEBUG)\\datatype\\parse\\parse-list.o: datatype\\parse\\parse-list.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c datatype\\parse\\parse-list.cpp -o $(OBJDIR_DEBUG)\\datatype\\parse\\parse-list.o

$(OBJDIR_DEBUG)\\datatype\\parse\\parse-number.o: datatype\\parse\\parse-number.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c datatype\\parse\\parse-number.cpp -o $(OBJDIR_DEBUG)\\datatype\\parse\\parse-number.o

$(OBJDIR_DEBUG)\\datatype\\parse\\parse-numlist.o: datatype\\parse\\parse-numlist.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c datatype\\parse\\parse-numlist.cpp -o $(OBJDIR_DEBUG)\\datatype\\parse\\parse-numlist.o

$(OBJDIR_DEBUG)\\datatype\\parse\\parse-period.o: datatype\\parse\\parse-period.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c datatype\\parse\\parse-period.cpp -o $(OBJDIR_DEBUG)\\datatype\\parse\\parse-period.o

$(OBJDIR_DEBUG)\\datatype\\parse\\parse-string.o: datatype\\parse\\parse-string.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c datatype\\parse\\parse-string.cpp -o $(OBJDIR_DEBUG)\\datatype\\parse\\parse-string.o

$(OBJDIR_DEBUG)\\datatype\\parse\\parse-time.o: datatype\\parse\\parse-time.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c datatype\\parse\\parse-time.cpp -o $(OBJDIR_DEBUG)\\datatype\\parse\\parse-time.o

$(OBJDIR_DEBUG)\\datatype\\parse\\parse-timlist.o: datatype\\parse\\parse-timlist.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c datatype\\parse\\parse-timlist.cpp -o $(OBJDIR_DEBUG)\\datatype\\parse\\parse-timlist.o

$(OBJDIR_DEBUG)\\datatype\\generator\\gen-period.o: datatype\\generator\\gen-period.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c datatype\\generator\\gen-period.cpp -o $(OBJDIR_DEBUG)\\datatype\\generator\\gen-period.o

$(OBJDIR_DEBUG)\\datatype\\generator\\gen-string.o: datatype\\generator\\gen-string.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c datatype\\generator\\gen-string.cpp -o $(OBJDIR_DEBUG)\\datatype\\generator\\gen-string.o

$(OBJDIR_DEBUG)\\datatype\\generator\\gen-time.o: datatype\\generator\\gen-time.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c datatype\\generator\\gen-time.cpp -o $(OBJDIR_DEBUG)\\datatype\\generator\\gen-time.o

$(OBJDIR_DEBUG)\\datatype\\generator\\gen-timlist.o: datatype\\generator\\gen-timlist.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c datatype\\generator\\gen-timlist.cpp -o $(OBJDIR_DEBUG)\\datatype\\generator\\gen-timlist.o

$(OBJDIR_DEBUG)\\datatype\\generator\\like.o: datatype\\generator\\like.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c datatype\\generator\\like.cpp -o $(OBJDIR_DEBUG)\\datatype\\generator\\like.o

$(OBJDIR_DEBUG)\\datatype\\number.o: datatype\\number.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c datatype\\number.cpp -o $(OBJDIR_DEBUG)\\datatype\\number.o

$(OBJDIR_DEBUG)\\datatype\\order.o: datatype\\order.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c datatype\\order.cpp -o $(OBJDIR_DEBUG)\\datatype\\order.o

$(OBJDIR_DEBUG)\\datatype\\parse-gen.o: datatype\\parse-gen.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c datatype\\parse-gen.cpp -o $(OBJDIR_DEBUG)\\datatype\\parse-gen.o

$(OBJDIR_DEBUG)\\datatype\\parse\\parse-bool.o: datatype\\parse\\parse-bool.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c datatype\\parse\\parse-bool.cpp -o $(OBJDIR_DEBUG)\\datatype\\parse\\parse-bool.o

$(OBJDIR_DEBUG)\\token.o: token.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c token.cpp -o $(OBJDIR_DEBUG)\\token.o

$(OBJDIR_DEBUG)\\tokens.o: tokens.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c tokens.cpp -o $(OBJDIR_DEBUG)\\tokens.o

$(OBJDIR_DEBUG)\\uroboros.o: uroboros.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c uroboros.cpp -o $(OBJDIR_DEBUG)\\uroboros.o

$(OBJDIR_DEBUG)\\util.o: util.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c util.cpp -o $(OBJDIR_DEBUG)\\util.o

$(OBJDIR_DEBUG)\\var\\var-group.o: var\\var-group.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c var\\var-group.cpp -o $(OBJDIR_DEBUG)\\var\\var-group.o

$(OBJDIR_DEBUG)\\var\\var-inner.o: var\\var-inner.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c var\\var-inner.cpp -o $(OBJDIR_DEBUG)\\var\\var-inner.o

$(OBJDIR_DEBUG)\\var\\var-runtime.o: var\\var-runtime.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c var\\var-runtime.cpp -o $(OBJDIR_DEBUG)\\var\\var-runtime.o

$(OBJDIR_DEBUG)\\var\\var.o: var\\var.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c var\\var.cpp -o $(OBJDIR_DEBUG)\\var\\var.o

$(OBJDIR_DEBUG)\\wndres.o: wndres.rc
	$(WINDRES) -i wndres.rc -J rc -o $(OBJDIR_DEBUG)\\wndres.o -O coff $(INC_DEBUG)

$(OBJDIR_DEBUG)\\datatype\\period.o: datatype\\period.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c datatype\\period.cpp -o $(OBJDIR_DEBUG)\\datatype\\period.o

$(OBJDIR_DEBUG)\\datatype\\primitives.o: datatype\\primitives.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c datatype\\primitives.cpp -o $(OBJDIR_DEBUG)\\datatype\\primitives.o

$(OBJDIR_DEBUG)\\datatype\\time.o: datatype\\time.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c datatype\\time.cpp -o $(OBJDIR_DEBUG)\\datatype\\time.o

$(OBJDIR_DEBUG)\\exception.o: exception.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c exception.cpp -o $(OBJDIR_DEBUG)\\exception.o

$(OBJDIR_DEBUG)\\hash.o: hash.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c hash.cpp -o $(OBJDIR_DEBUG)\\hash.o

$(OBJDIR_DEBUG)\\keyword.o: keyword.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c keyword.cpp -o $(OBJDIR_DEBUG)\\keyword.o

$(OBJDIR_DEBUG)\\lexer.o: lexer.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c lexer.cpp -o $(OBJDIR_DEBUG)\\lexer.o

$(OBJDIR_DEBUG)\\main.o: main.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c main.cpp -o $(OBJDIR_DEBUG)\\main.o

$(OBJDIR_DEBUG)\\os.o: os.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c os.cpp -o $(OBJDIR_DEBUG)\\os.o

$(OBJDIR_DEBUG)\\print.o: print.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c print.cpp -o $(OBJDIR_DEBUG)\\print.o

$(OBJDIR_DEBUG)\\command\\com-parse-kw.o: command\\com-parse-kw.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c command\\com-parse-kw.cpp -o $(OBJDIR_DEBUG)\\command\\com-parse-kw.o

$(OBJDIR_DEBUG)\\command\\com-parse-unit.o: command\\com-parse-unit.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c command\\com-parse-unit.cpp -o $(OBJDIR_DEBUG)\\command\\com-parse-unit.o

$(OBJDIR_DEBUG)\\command\\com-parse.o: command\\com-parse.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c command\\com-parse.cpp -o $(OBJDIR_DEBUG)\\command\\com-parse.o

$(OBJDIR_DEBUG)\\command\\com-struct.o: command\\com-struct.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c command\\com-struct.cpp -o $(OBJDIR_DEBUG)\\command\\com-struct.o

$(OBJDIR_DEBUG)\\command\\com-var.o: command\\com-var.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c command\\com-var.cpp -o $(OBJDIR_DEBUG)\\command\\com-var.o

$(OBJDIR_DEBUG)\\command\\com.o: command\\com.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c command\\com.cpp -o $(OBJDIR_DEBUG)\\command\\com.o

$(OBJDIR_DEBUG)\\datatype\\cast.o: datatype\\cast.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c datatype\\cast.cpp -o $(OBJDIR_DEBUG)\\datatype\\cast.o

$(OBJDIR_DEBUG)\\datatype\\def-var.o: datatype\\def-var.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c datatype\\def-var.cpp -o $(OBJDIR_DEBUG)\\datatype\\def-var.o

$(OBJDIR_DEBUG)\\attr-memory.o: attr-memory.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c attr-memory.cpp -o $(OBJDIR_DEBUG)\\attr-memory.o

$(OBJDIR_DEBUG)\\attribute.o: attribute.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c attribute.cpp -o $(OBJDIR_DEBUG)\\attribute.o

$(OBJDIR_DEBUG)\\brackets.o: brackets.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c brackets.cpp -o $(OBJDIR_DEBUG)\\brackets.o

$(OBJDIR_DEBUG)\\command\\aggregate.o: command\\aggregate.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c command\\aggregate.cpp -o $(OBJDIR_DEBUG)\\command\\aggregate.o

$(OBJDIR_DEBUG)\\command\\com-arg.o: command\\com-arg.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c command\\com-arg.cpp -o $(OBJDIR_DEBUG)\\command\\com-arg.o

$(OBJDIR_DEBUG)\\command\\com-condition.o: command\\com-condition.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c command\\com-condition.cpp -o $(OBJDIR_DEBUG)\\command\\com-condition.o

$(OBJDIR_DEBUG)\\command\\com-core-aggr.o: command\\com-core-aggr.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c command\\com-core-aggr.cpp -o $(OBJDIR_DEBUG)\\command\\com-core-aggr.o

$(OBJDIR_DEBUG)\\command\\com-core.o: command\\com-core.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c command\\com-core.cpp -o $(OBJDIR_DEBUG)\\command\\com-core.o

$(OBJDIR_DEBUG)\\command\\com-create.o: command\\com-create.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c command\\com-create.cpp -o $(OBJDIR_DEBUG)\\command\\com-create.o

$(OBJDIR_DEBUG)\\command\\com-misc.o: command\\com-misc.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c command\\com-misc.cpp -o $(OBJDIR_DEBUG)\\command\\com-misc.o

$(OBJDIR_DEBUG)\\datatype\\function\\func-time.o: datatype\\function\\func-time.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c datatype\\function\\func-time.cpp -o $(OBJDIR_DEBUG)\\datatype\\function\\func-time.o

$(OBJDIR_DEBUG)\\datatype\\generator.o: datatype\\generator.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c datatype\\generator.cpp -o $(OBJDIR_DEBUG)\\datatype\\generator.o

$(OBJDIR_DEBUG)\\datatype\\generator\\gen-bool-compare.o: datatype\\generator\\gen-bool-compare.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c datatype\\generator\\gen-bool-compare.cpp -o $(OBJDIR_DEBUG)\\datatype\\generator\\gen-bool-compare.o

$(OBJDIR_DEBUG)\\datatype\\generator\\gen-bool.o: datatype\\generator\\gen-bool.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c datatype\\generator\\gen-bool.cpp -o $(OBJDIR_DEBUG)\\datatype\\generator\\gen-bool.o

$(OBJDIR_DEBUG)\\datatype\\generator\\gen-definition.o: datatype\\generator\\gen-definition.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c datatype\\generator\\gen-definition.cpp -o $(OBJDIR_DEBUG)\\datatype\\generator\\gen-definition.o

$(OBJDIR_DEBUG)\\datatype\\generator\\gen-list.o: datatype\\generator\\gen-list.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c datatype\\generator\\gen-list.cpp -o $(OBJDIR_DEBUG)\\datatype\\generator\\gen-list.o

$(OBJDIR_DEBUG)\\datatype\\generator\\gen-number.o: datatype\\generator\\gen-number.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c datatype\\generator\\gen-number.cpp -o $(OBJDIR_DEBUG)\\datatype\\generator\\gen-number.o

$(OBJDIR_DEBUG)\\datatype\\generator\\gen-numlist.o: datatype\\generator\\gen-numlist.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c datatype\\generator\\gen-numlist.cpp -o $(OBJDIR_DEBUG)\\datatype\\generator\\gen-numlist.o

$(OBJDIR_DEBUG)\\datatype\\generator\\gen-os.o: datatype\\generator\\gen-os.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c datatype\\generator\\gen-os.cpp -o $(OBJDIR_DEBUG)\\datatype\\generator\\gen-os.o

$(OBJDIR_DEBUG)\\datatype\\definition.o: datatype\\definition.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c datatype\\definition.cpp -o $(OBJDIR_DEBUG)\\datatype\\definition.o

$(OBJDIR_DEBUG)\\datatype\\function.o: datatype\\function.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c datatype\\function.cpp -o $(OBJDIR_DEBUG)\\datatype\\function.o

$(OBJDIR_DEBUG)\\datatype\\function\\func-aggr.o: datatype\\function\\func-aggr.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c datatype\\function\\func-aggr.cpp -o $(OBJDIR_DEBUG)\\datatype\\function\\func-aggr.o

$(OBJDIR_DEBUG)\\datatype\\function\\func-attr.o: datatype\\function\\func-attr.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c datatype\\function\\func-attr.cpp -o $(OBJDIR_DEBUG)\\datatype\\function\\func-attr.o

$(OBJDIR_DEBUG)\\datatype\\function\\func-bool.o: datatype\\function\\func-bool.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c datatype\\function\\func-bool.cpp -o $(OBJDIR_DEBUG)\\datatype\\function\\func-bool.o

$(OBJDIR_DEBUG)\\datatype\\function\\func-list.o: datatype\\function\\func-list.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c datatype\\function\\func-list.cpp -o $(OBJDIR_DEBUG)\\datatype\\function\\func-list.o

$(OBJDIR_DEBUG)\\datatype\\function\\func-number.o: datatype\\function\\func-number.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c datatype\\function\\func-number.cpp -o $(OBJDIR_DEBUG)\\datatype\\function\\func-number.o

$(OBJDIR_DEBUG)\\datatype\\function\\func-string.o: datatype\\function\\func-string.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c datatype\\function\\func-string.cpp -o $(OBJDIR_DEBUG)\\datatype\\function\\func-string.o

clean_debug: 
	cmd /c del /f $(OBJ_DEBUG) $(OUT_DEBUG)
	cmd /c rd bin\\Debug
	cmd /c rd $(OBJDIR_DEBUG)\\datatype\\parse
	cmd /c rd $(OBJDIR_DEBUG)\\datatype\\generator
	cmd /c rd $(OBJDIR_DEBUG)\\datatype
	cmd /c rd $(OBJDIR_DEBUG)
	cmd /c rd $(OBJDIR_DEBUG)\\var
	cmd /c rd $(OBJDIR_DEBUG)\\command
	cmd /c rd $(OBJDIR_DEBUG)\\datatype\\function

before_release: 
	cmd /c if not exist bin\\Release md bin\\Release
	cmd /c if not exist $(OBJDIR_RELEASE)\\datatype\\parse md $(OBJDIR_RELEASE)\\datatype\\parse
	cmd /c if not exist $(OBJDIR_RELEASE)\\datatype\\generator md $(OBJDIR_RELEASE)\\datatype\\generator
	cmd /c if not exist $(OBJDIR_RELEASE)\\datatype md $(OBJDIR_RELEASE)\\datatype
	cmd /c if not exist $(OBJDIR_RELEASE) md $(OBJDIR_RELEASE)
	cmd /c if not exist $(OBJDIR_RELEASE)\\var md $(OBJDIR_RELEASE)\\var
	cmd /c if not exist $(OBJDIR_RELEASE)\\command md $(OBJDIR_RELEASE)\\command
	cmd /c if not exist $(OBJDIR_RELEASE)\\datatype\\function md $(OBJDIR_RELEASE)\\datatype\\function

after_release: 

release: before_release out_release after_release

out_release: before_release $(OBJ_RELEASE) $(DEP_RELEASE)
	$(LD) $(LIBDIR_RELEASE) -o $(OUT_RELEASE) $(OBJ_RELEASE)  $(LDFLAGS_RELEASE) $(LIB_RELEASE)

$(OBJDIR_RELEASE)\\datatype\\parse\\parse-definition.o: datatype\\parse\\parse-definition.cpp
	$(CXX) $(CFLAGS_RELEASE) $(INC_RELEASE) -c datatype\\parse\\parse-definition.cpp -o $(OBJDIR_RELEASE)\\datatype\\parse\\parse-definition.o

$(OBJDIR_RELEASE)\\datatype\\parse\\parse-generic.o: datatype\\parse\\parse-generic.cpp
	$(CXX) $(CFLAGS_RELEASE) $(INC_RELEASE) -c datatype\\parse\\parse-generic.cpp -o $(OBJDIR_RELEASE)\\datatype\\parse\\parse-generic.o

$(OBJDIR_RELEASE)\\datatype\\parse\\parse-list.o: datatype\\parse\\parse-list.cpp
	$(CXX) $(CFLAGS_RELEASE) $(INC_RELEASE) -c datatype\\parse\\parse-list.cpp -o $(OBJDIR_RELEASE)\\datatype\\parse\\parse-list.o

$(OBJDIR_RELEASE)\\datatype\\parse\\parse-number.o: datatype\\parse\\parse-number.cpp
	$(CXX) $(CFLAGS_RELEASE) $(INC_RELEASE) -c datatype\\parse\\parse-number.cpp -o $(OBJDIR_RELEASE)\\datatype\\parse\\parse-number.o

$(OBJDIR_RELEASE)\\datatype\\parse\\parse-numlist.o: datatype\\parse\\parse-numlist.cpp
	$(CXX) $(CFLAGS_RELEASE) $(INC_RELEASE) -c datatype\\parse\\parse-numlist.cpp -o $(OBJDIR_RELEASE)\\datatype\\parse\\parse-numlist.o

$(OBJDIR_RELEASE)\\datatype\\parse\\parse-period.o: datatype\\parse\\parse-period.cpp
	$(CXX) $(CFLAGS_RELEASE) $(INC_RELEASE) -c datatype\\parse\\parse-period.cpp -o $(OBJDIR_RELEASE)\\datatype\\parse\\parse-period.o

$(OBJDIR_RELEASE)\\datatype\\parse\\parse-string.o: datatype\\parse\\parse-string.cpp
	$(CXX) $(CFLAGS_RELEASE) $(INC_RELEASE) -c datatype\\parse\\parse-string.cpp -o $(OBJDIR_RELEASE)\\datatype\\parse\\parse-string.o

$(OBJDIR_RELEASE)\\datatype\\parse\\parse-time.o: datatype\\parse\\parse-time.cpp
	$(CXX) $(CFLAGS_RELEASE) $(INC_RELEASE) -c datatype\\parse\\parse-time.cpp -o $(OBJDIR_RELEASE)\\datatype\\parse\\parse-time.o

$(OBJDIR_RELEASE)\\datatype\\parse\\parse-timlist.o: datatype\\parse\\parse-timlist.cpp
	$(CXX) $(CFLAGS_RELEASE) $(INC_RELEASE) -c datatype\\parse\\parse-timlist.cpp -o $(OBJDIR_RELEASE)\\datatype\\parse\\parse-timlist.o

$(OBJDIR_RELEASE)\\datatype\\generator\\gen-period.o: datatype\\generator\\gen-period.cpp
	$(CXX) $(CFLAGS_RELEASE) $(INC_RELEASE) -c datatype\\generator\\gen-period.cpp -o $(OBJDIR_RELEASE)\\datatype\\generator\\gen-period.o

$(OBJDIR_RELEASE)\\datatype\\generator\\gen-string.o: datatype\\generator\\gen-string.cpp
	$(CXX) $(CFLAGS_RELEASE) $(INC_RELEASE) -c datatype\\generator\\gen-string.cpp -o $(OBJDIR_RELEASE)\\datatype\\generator\\gen-string.o

$(OBJDIR_RELEASE)\\datatype\\generator\\gen-time.o: datatype\\generator\\gen-time.cpp
	$(CXX) $(CFLAGS_RELEASE) $(INC_RELEASE) -c datatype\\generator\\gen-time.cpp -o $(OBJDIR_RELEASE)\\datatype\\generator\\gen-time.o

$(OBJDIR_RELEASE)\\datatype\\generator\\gen-timlist.o: datatype\\generator\\gen-timlist.cpp
	$(CXX) $(CFLAGS_RELEASE) $(INC_RELEASE) -c datatype\\generator\\gen-timlist.cpp -o $(OBJDIR_RELEASE)\\datatype\\generator\\gen-timlist.o

$(OBJDIR_RELEASE)\\datatype\\generator\\like.o: datatype\\generator\\like.cpp
	$(CXX) $(CFLAGS_RELEASE) $(INC_RELEASE) -c datatype\\generator\\like.cpp -o $(OBJDIR_RELEASE)\\datatype\\generator\\like.o

$(OBJDIR_RELEASE)\\datatype\\number.o: datatype\\number.cpp
	$(CXX) $(CFLAGS_RELEASE) $(INC_RELEASE) -c datatype\\number.cpp -o $(OBJDIR_RELEASE)\\datatype\\number.o

$(OBJDIR_RELEASE)\\datatype\\order.o: datatype\\order.cpp
	$(CXX) $(CFLAGS_RELEASE) $(INC_RELEASE) -c datatype\\order.cpp -o $(OBJDIR_RELEASE)\\datatype\\order.o

$(OBJDIR_RELEASE)\\datatype\\parse-gen.o: datatype\\parse-gen.cpp
	$(CXX) $(CFLAGS_RELEASE) $(INC_RELEASE) -c datatype\\parse-gen.cpp -o $(OBJDIR_RELEASE)\\datatype\\parse-gen.o

$(OBJDIR_RELEASE)\\datatype\\parse\\parse-bool.o: datatype\\parse\\parse-bool.cpp
	$(CXX) $(CFLAGS_RELEASE) $(INC_RELEASE) -c datatype\\parse\\parse-bool.cpp -o $(OBJDIR_RELEASE)\\datatype\\parse\\parse-bool.o

$(OBJDIR_RELEASE)\\token.o: token.cpp
	$(CXX) $(CFLAGS_RELEASE) $(INC_RELEASE) -c token.cpp -o $(OBJDIR_RELEASE)\\token.o

$(OBJDIR_RELEASE)\\tokens.o: tokens.cpp
	$(CXX) $(CFLAGS_RELEASE) $(INC_RELEASE) -c tokens.cpp -o $(OBJDIR_RELEASE)\\tokens.o

$(OBJDIR_RELEASE)\\uroboros.o: uroboros.cpp
	$(CXX) $(CFLAGS_RELEASE) $(INC_RELEASE) -c uroboros.cpp -o $(OBJDIR_RELEASE)\\uroboros.o

$(OBJDIR_RELEASE)\\util.o: util.cpp
	$(CXX) $(CFLAGS_RELEASE) $(INC_RELEASE) -c util.cpp -o $(OBJDIR_RELEASE)\\util.o

$(OBJDIR_RELEASE)\\var\\var-group.o: var\\var-group.cpp
	$(CXX) $(CFLAGS_RELEASE) $(INC_RELEASE) -c var\\var-group.cpp -o $(OBJDIR_RELEASE)\\var\\var-group.o

$(OBJDIR_RELEASE)\\var\\var-inner.o: var\\var-inner.cpp
	$(CXX) $(CFLAGS_RELEASE) $(INC_RELEASE) -c var\\var-inner.cpp -o $(OBJDIR_RELEASE)\\var\\var-inner.o

$(OBJDIR_RELEASE)\\var\\var-runtime.o: var\\var-runtime.cpp
	$(CXX) $(CFLAGS_RELEASE) $(INC_RELEASE) -c var\\var-runtime.cpp -o $(OBJDIR_RELEASE)\\var\\var-runtime.o

$(OBJDIR_RELEASE)\\var\\var.o: var\\var.cpp
	$(CXX) $(CFLAGS_RELEASE) $(INC_RELEASE) -c var\\var.cpp -o $(OBJDIR_RELEASE)\\var\\var.o

$(OBJDIR_RELEASE)\\wndres.o: wndres.rc
	$(WINDRES) -i wndres.rc -J rc -o $(OBJDIR_RELEASE)\\wndres.o -O coff $(INC_RELEASE)

$(OBJDIR_RELEASE)\\datatype\\period.o: datatype\\period.cpp
	$(CXX) $(CFLAGS_RELEASE) $(INC_RELEASE) -c datatype\\period.cpp -o $(OBJDIR_RELEASE)\\datatype\\period.o

$(OBJDIR_RELEASE)\\datatype\\primitives.o: datatype\\primitives.cpp
	$(CXX) $(CFLAGS_RELEASE) $(INC_RELEASE) -c datatype\\primitives.cpp -o $(OBJDIR_RELEASE)\\datatype\\primitives.o

$(OBJDIR_RELEASE)\\datatype\\time.o: datatype\\time.cpp
	$(CXX) $(CFLAGS_RELEASE) $(INC_RELEASE) -c datatype\\time.cpp -o $(OBJDIR_RELEASE)\\datatype\\time.o

$(OBJDIR_RELEASE)\\exception.o: exception.cpp
	$(CXX) $(CFLAGS_RELEASE) $(INC_RELEASE) -c exception.cpp -o $(OBJDIR_RELEASE)\\exception.o

$(OBJDIR_RELEASE)\\hash.o: hash.cpp
	$(CXX) $(CFLAGS_RELEASE) $(INC_RELEASE) -c hash.cpp -o $(OBJDIR_RELEASE)\\hash.o

$(OBJDIR_RELEASE)\\keyword.o: keyword.cpp
	$(CXX) $(CFLAGS_RELEASE) $(INC_RELEASE) -c keyword.cpp -o $(OBJDIR_RELEASE)\\keyword.o

$(OBJDIR_RELEASE)\\lexer.o: lexer.cpp
	$(CXX) $(CFLAGS_RELEASE) $(INC_RELEASE) -c lexer.cpp -o $(OBJDIR_RELEASE)\\lexer.o

$(OBJDIR_RELEASE)\\main.o: main.cpp
	$(CXX) $(CFLAGS_RELEASE) $(INC_RELEASE) -c main.cpp -o $(OBJDIR_RELEASE)\\main.o

$(OBJDIR_RELEASE)\\os.o: os.cpp
	$(CXX) $(CFLAGS_RELEASE) $(INC_RELEASE) -c os.cpp -o $(OBJDIR_RELEASE)\\os.o

$(OBJDIR_RELEASE)\\print.o: print.cpp
	$(CXX) $(CFLAGS_RELEASE) $(INC_RELEASE) -c print.cpp -o $(OBJDIR_RELEASE)\\print.o

$(OBJDIR_RELEASE)\\command\\com-parse-kw.o: command\\com-parse-kw.cpp
	$(CXX) $(CFLAGS_RELEASE) $(INC_RELEASE) -c command\\com-parse-kw.cpp -o $(OBJDIR_RELEASE)\\command\\com-parse-kw.o

$(OBJDIR_RELEASE)\\command\\com-parse-unit.o: command\\com-parse-unit.cpp
	$(CXX) $(CFLAGS_RELEASE) $(INC_RELEASE) -c command\\com-parse-unit.cpp -o $(OBJDIR_RELEASE)\\command\\com-parse-unit.o

$(OBJDIR_RELEASE)\\command\\com-parse.o: command\\com-parse.cpp
	$(CXX) $(CFLAGS_RELEASE) $(INC_RELEASE) -c command\\com-parse.cpp -o $(OBJDIR_RELEASE)\\command\\com-parse.o

$(OBJDIR_RELEASE)\\command\\com-struct.o: command\\com-struct.cpp
	$(CXX) $(CFLAGS_RELEASE) $(INC_RELEASE) -c command\\com-struct.cpp -o $(OBJDIR_RELEASE)\\command\\com-struct.o

$(OBJDIR_RELEASE)\\command\\com-var.o: command\\com-var.cpp
	$(CXX) $(CFLAGS_RELEASE) $(INC_RELEASE) -c command\\com-var.cpp -o $(OBJDIR_RELEASE)\\command\\com-var.o

$(OBJDIR_RELEASE)\\command\\com.o: command\\com.cpp
	$(CXX) $(CFLAGS_RELEASE) $(INC_RELEASE) -c command\\com.cpp -o $(OBJDIR_RELEASE)\\command\\com.o

$(OBJDIR_RELEASE)\\datatype\\cast.o: datatype\\cast.cpp
	$(CXX) $(CFLAGS_RELEASE) $(INC_RELEASE) -c datatype\\cast.cpp -o $(OBJDIR_RELEASE)\\datatype\\cast.o

$(OBJDIR_RELEASE)\\datatype\\def-var.o: datatype\\def-var.cpp
	$(CXX) $(CFLAGS_RELEASE) $(INC_RELEASE) -c datatype\\def-var.cpp -o $(OBJDIR_RELEASE)\\datatype\\def-var.o

$(OBJDIR_RELEASE)\\attr-memory.o: attr-memory.cpp
	$(CXX) $(CFLAGS_RELEASE) $(INC_RELEASE) -c attr-memory.cpp -o $(OBJDIR_RELEASE)\\attr-memory.o

$(OBJDIR_RELEASE)\\attribute.o: attribute.cpp
	$(CXX) $(CFLAGS_RELEASE) $(INC_RELEASE) -c attribute.cpp -o $(OBJDIR_RELEASE)\\attribute.o

$(OBJDIR_RELEASE)\\brackets.o: brackets.cpp
	$(CXX) $(CFLAGS_RELEASE) $(INC_RELEASE) -c brackets.cpp -o $(OBJDIR_RELEASE)\\brackets.o

$(OBJDIR_RELEASE)\\command\\aggregate.o: command\\aggregate.cpp
	$(CXX) $(CFLAGS_RELEASE) $(INC_RELEASE) -c command\\aggregate.cpp -o $(OBJDIR_RELEASE)\\command\\aggregate.o

$(OBJDIR_RELEASE)\\command\\com-arg.o: command\\com-arg.cpp
	$(CXX) $(CFLAGS_RELEASE) $(INC_RELEASE) -c command\\com-arg.cpp -o $(OBJDIR_RELEASE)\\command\\com-arg.o

$(OBJDIR_RELEASE)\\command\\com-condition.o: command\\com-condition.cpp
	$(CXX) $(CFLAGS_RELEASE) $(INC_RELEASE) -c command\\com-condition.cpp -o $(OBJDIR_RELEASE)\\command\\com-condition.o

$(OBJDIR_RELEASE)\\command\\com-core-aggr.o: command\\com-core-aggr.cpp
	$(CXX) $(CFLAGS_RELEASE) $(INC_RELEASE) -c command\\com-core-aggr.cpp -o $(OBJDIR_RELEASE)\\command\\com-core-aggr.o

$(OBJDIR_RELEASE)\\command\\com-core.o: command\\com-core.cpp
	$(CXX) $(CFLAGS_RELEASE) $(INC_RELEASE) -c command\\com-core.cpp -o $(OBJDIR_RELEASE)\\command\\com-core.o

$(OBJDIR_RELEASE)\\command\\com-create.o: command\\com-create.cpp
	$(CXX) $(CFLAGS_RELEASE) $(INC_RELEASE) -c command\\com-create.cpp -o $(OBJDIR_RELEASE)\\command\\com-create.o

$(OBJDIR_RELEASE)\\command\\com-misc.o: command\\com-misc.cpp
	$(CXX) $(CFLAGS_RELEASE) $(INC_RELEASE) -c command\\com-misc.cpp -o $(OBJDIR_RELEASE)\\command\\com-misc.o

$(OBJDIR_RELEASE)\\datatype\\function\\func-time.o: datatype\\function\\func-time.cpp
	$(CXX) $(CFLAGS_RELEASE) $(INC_RELEASE) -c datatype\\function\\func-time.cpp -o $(OBJDIR_RELEASE)\\datatype\\function\\func-time.o

$(OBJDIR_RELEASE)\\datatype\\generator.o: datatype\\generator.cpp
	$(CXX) $(CFLAGS_RELEASE) $(INC_RELEASE) -c datatype\\generator.cpp -o $(OBJDIR_RELEASE)\\datatype\\generator.o

$(OBJDIR_RELEASE)\\datatype\\generator\\gen-bool-compare.o: datatype\\generator\\gen-bool-compare.cpp
	$(CXX) $(CFLAGS_RELEASE) $(INC_RELEASE) -c datatype\\generator\\gen-bool-compare.cpp -o $(OBJDIR_RELEASE)\\datatype\\generator\\gen-bool-compare.o

$(OBJDIR_RELEASE)\\datatype\\generator\\gen-bool.o: datatype\\generator\\gen-bool.cpp
	$(CXX) $(CFLAGS_RELEASE) $(INC_RELEASE) -c datatype\\generator\\gen-bool.cpp -o $(OBJDIR_RELEASE)\\datatype\\generator\\gen-bool.o

$(OBJDIR_RELEASE)\\datatype\\generator\\gen-definition.o: datatype\\generator\\gen-definition.cpp
	$(CXX) $(CFLAGS_RELEASE) $(INC_RELEASE) -c datatype\\generator\\gen-definition.cpp -o $(OBJDIR_RELEASE)\\datatype\\generator\\gen-definition.o

$(OBJDIR_RELEASE)\\datatype\\generator\\gen-list.o: datatype\\generator\\gen-list.cpp
	$(CXX) $(CFLAGS_RELEASE) $(INC_RELEASE) -c datatype\\generator\\gen-list.cpp -o $(OBJDIR_RELEASE)\\datatype\\generator\\gen-list.o

$(OBJDIR_RELEASE)\\datatype\\generator\\gen-number.o: datatype\\generator\\gen-number.cpp
	$(CXX) $(CFLAGS_RELEASE) $(INC_RELEASE) -c datatype\\generator\\gen-number.cpp -o $(OBJDIR_RELEASE)\\datatype\\generator\\gen-number.o

$(OBJDIR_RELEASE)\\datatype\\generator\\gen-numlist.o: datatype\\generator\\gen-numlist.cpp
	$(CXX) $(CFLAGS_RELEASE) $(INC_RELEASE) -c datatype\\generator\\gen-numlist.cpp -o $(OBJDIR_RELEASE)\\datatype\\generator\\gen-numlist.o

$(OBJDIR_RELEASE)\\datatype\\generator\\gen-os.o: datatype\\generator\\gen-os.cpp
	$(CXX) $(CFLAGS_RELEASE) $(INC_RELEASE) -c datatype\\generator\\gen-os.cpp -o $(OBJDIR_RELEASE)\\datatype\\generator\\gen-os.o

$(OBJDIR_RELEASE)\\datatype\\definition.o: datatype\\definition.cpp
	$(CXX) $(CFLAGS_RELEASE) $(INC_RELEASE) -c datatype\\definition.cpp -o $(OBJDIR_RELEASE)\\datatype\\definition.o

$(OBJDIR_RELEASE)\\datatype\\function.o: datatype\\function.cpp
	$(CXX) $(CFLAGS_RELEASE) $(INC_RELEASE) -c datatype\\function.cpp -o $(OBJDIR_RELEASE)\\datatype\\function.o

$(OBJDIR_RELEASE)\\datatype\\function\\func-aggr.o: datatype\\function\\func-aggr.cpp
	$(CXX) $(CFLAGS_RELEASE) $(INC_RELEASE) -c datatype\\function\\func-aggr.cpp -o $(OBJDIR_RELEASE)\\datatype\\function\\func-aggr.o

$(OBJDIR_RELEASE)\\datatype\\function\\func-attr.o: datatype\\function\\func-attr.cpp
	$(CXX) $(CFLAGS_RELEASE) $(INC_RELEASE) -c datatype\\function\\func-attr.cpp -o $(OBJDIR_RELEASE)\\datatype\\function\\func-attr.o

$(OBJDIR_RELEASE)\\datatype\\function\\func-bool.o: datatype\\function\\func-bool.cpp
	$(CXX) $(CFLAGS_RELEASE) $(INC_RELEASE) -c datatype\\function\\func-bool.cpp -o $(OBJDIR_RELEASE)\\datatype\\function\\func-bool.o

$(OBJDIR_RELEASE)\\datatype\\function\\func-list.o: datatype\\function\\func-list.cpp
	$(CXX) $(CFLAGS_RELEASE) $(INC_RELEASE) -c datatype\\function\\func-list.cpp -o $(OBJDIR_RELEASE)\\datatype\\function\\func-list.o

$(OBJDIR_RELEASE)\\datatype\\function\\func-number.o: datatype\\function\\func-number.cpp
	$(CXX) $(CFLAGS_RELEASE) $(INC_RELEASE) -c datatype\\function\\func-number.cpp -o $(OBJDIR_RELEASE)\\datatype\\function\\func-number.o

$(OBJDIR_RELEASE)\\datatype\\function\\func-string.o: datatype\\function\\func-string.cpp
	$(CXX) $(CFLAGS_RELEASE) $(INC_RELEASE) -c datatype\\function\\func-string.cpp -o $(OBJDIR_RELEASE)\\datatype\\function\\func-string.o

clean_release: 
	cmd /c del /f $(OBJ_RELEASE) $(OUT_RELEASE)
	cmd /c rd bin\\Release
	cmd /c rd $(OBJDIR_RELEASE)\\datatype\\parse
	cmd /c rd $(OBJDIR_RELEASE)\\datatype\\generator
	cmd /c rd $(OBJDIR_RELEASE)\\datatype
	cmd /c rd $(OBJDIR_RELEASE)
	cmd /c rd $(OBJDIR_RELEASE)\\var
	cmd /c rd $(OBJDIR_RELEASE)\\command
	cmd /c rd $(OBJDIR_RELEASE)\\datatype\\function

.PHONY: before_debug after_debug clean_debug before_release after_release clean_release

