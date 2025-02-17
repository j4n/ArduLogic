
CXXFLAGS += -MD -Wall -Os -ggdb
CXX = g++

LDLIBS += -lstdc++ -lm

ardulogic: ardulogic.o parser.o lexer.o genfirmware.o readdata.o \
		writevcd.o rawfile.o decode_jtag.o decode_spi.o

parser.cc parser.hh: parser.y
	bison -o parser.cc -d parser.y

lexer.cc: lexer.l
	flex -o lexer.cc lexer.l

clean:
	rm -f ardulogic *.d *.o core
	rm -f parser.cc parser.hh lexer.cc
	rm -f .ardulogic_tmp.firmware.*

-include *.d

