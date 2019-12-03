CXX=g++

CSVLIBDIR=libcsv-3.0.3
CSVLIB=libcsv.a

INCDIR=./include
SRCDIR=./src
BINDIR=./bin
OBJDIR=./obj
TESTBINDIR=./testbin

CXXFLAGS = --std=c++14 -I $(INCDIR) -I $(CSVLIBDIR)
TESTLDFLAGS= -lgtest_main -lgtest -lpthread
XMLLDFLAGS= -lexpat

CSVOBJS=$(OBJDIR)/CSVReader.o $(OBJDIR)/CSVWriter.o
XMLOBJS=$(OBJDIR)/XMLReader.o $(OBJDIR)/XMLWriter.o
STRINGOBJ=$(OBJDIR)/StringUtils.o
ROUTEROBJ=$(OBJDIR)/MapRouter.o
PROJOBJS=$(OBJDIR)/CSVReader.o $(OBJDIR)/XMLReader.o $(OBJDIR)/MapRouter.o $(OBJDIR)/CSVWriter.o $(OBJDIR)/XMLWriter.o $(OBJDIR)/StringUtils.o
PROJ_NAME =proj5

CSVTEST=testcsv
XMLTEST=testxml
ROUTERTEST=testrouter

all: $(CSVLIBDIR)/.libs/$(CSVLIB) directories RUNTESTS

RUNTESTS: RUNCSVTEST RUNXMLTEST RUNROUTERTEST

RUNCSVTEST: $(TESTBINDIR)/$(CSVTEST)
	$(TESTBINDIR)/$(CSVTEST)

RUNXMLTEST: $(TESTBINDIR)/$(XMLTEST)
	$(TESTBINDIR)/$(XMLTEST)

RUNROUTERTEST: $(TESTBINDIR)/$(ROUTERTEST)
	$(TESTBINDIR)/$(ROUTERTEST)

$(CSVLIBDIR)/.libs/$(CSVLIB): $(CSVLIBDIR)/Makefile
	cd $(CSVLIBDIR); make ; cd ..

$(CSVLIBDIR)/Makefile:
	cd $(CSVLIBDIR); ./configure ; cd ..

$(BINDIR)/$(PROJ_NAME): $(PROJOBJS) $(OBJDIR)/main.o $(CSVLIBDIR)/.libs/$(CSVLIB)
	$(CXX) $(OBJDIR)/main.o $(PROJOBJS) $(CSVLIBDIR)/.libs/$(CSVLIB) -o $(BINDIR)/$(PROJ_NAME) $(CXXFLAGS) $(TESTLDFLAGS) $(XMLLDFLAGS)

$(TESTBINDIR)/$(ROUTERTEST): $(OBJDIR)/testrouter.o $(ROUTEROBJ) $(CSVOBJS) $(XMLOBJS) $(CSVLIBDIR)/.libs $(STRINGOBJ)
	$(CXX) $(CXXFLAGS) $(OBJDIR)/testrouter.o $(CSVOBJS) $(ROUTEROBJ) $(XMLOBJS) $(STRINGOBJ) $(CSVLIBDIR)/.libs/$(CSVLIB) -o $(TESTBINDIR)/$(ROUTERTEST) $(TESTLDFLAGS) $(XMLLDFLAGS)

$(OBJDIR)/main.o: $(SRCDIR)/main.cpp $(INCDIR)/CSVReader.h $(INCDIR)/MapRouter.h 
	$(CXX) $(CXXFLAGS)  $(SRCDIR)/main.cpp -c -o $(OBJDIR)/main.o

$(OBJDIR)/testrouter.o: $(SRCDIR)/testrouter.cpp $(INCDIR)/CSVReader.h $(INCDIR)/XMLReader.h $(INCDIR)/MapRouter.h
	$(CXX) $(CXXFLAGS) $(SRCDIR)/testrouter.cpp -c -o $(OBJDIR)/testrouter.o

$(OBJDIR)/MapRouter.o: $(SRCDIR)/MapRouter.cpp $(INCDIR)/MapRouter.h
	$(CXX) $(CXXFLAGS) $(SRCDIR)/MapRouter.cpp -c -o $(OBJDIR)/MapRouter.o

$(TESTBINDIR)/$(CSVTEST): $(OBJDIR)/testcsv.o $(CSVOBJS) $(CSVLIBDIR)/.libs $(STRINGOBJ)
	$(CXX) $(OBJDIR)/testcsv.o $(CSVOBJS) $(STRINGOBJ) $(CSVLIBDIR)/.libs/$(CSVLIB) -o $(TESTBINDIR)/$(CSVTEST) $(TESTLDFLAGS)

$(OBJDIR)/testcsv.o: $(SRCDIR)/testcsv.cpp $(INCDIR)/CSVReader.h
	$(CXX) $(CXXFLAGS) $(SRCDIR)/testcsv.cpp -c -o $(OBJDIR)/testcsv.o

$(OBJDIR)/CSVReader.o: $(SRCDIR)/CSVReader.cpp $(INCDIR)/CSVReader.h
	$(CXX) $(CXXFLAGS) $(SRCDIR)/CSVReader.cpp -c -o $(OBJDIR)/CSVReader.o 

$(OBJDIR)/CSVWriter.o: $(SRCDIR)/CSVWriter.cpp $(INCDIR)/CSVWriter.h
	$(CXX) $(CXXFLAGS) $(SRCDIR)/CSVWriter.cpp -c -o $(OBJDIR)/CSVWriter.o

$(OBJDIR)/StringUtils.o: $(SRCDIR)/StringUtils.cpp $(INCDIR)/StringUtils.h
	$(CXX) $(CXXFLAGS) $(SRCDIR)/StringUtils.cpp -c -o $(OBJDIR)/StringUtils.o

$(TESTBINDIR)/$(XMLTEST): $(OBJDIR)/testxml.o $(XMLOBJS) 
	$(CXX) $(OBJDIR)/testxml.o $(XMLOBJS) -o $(TESTBINDIR)/$(XMLTEST) $(TESTLDFLAGS) $(XMLLDFLAGS)

$(OBJDIR)/testxml.o: $(SRCDIR)/testxml.cpp $(INCDIR)/XMLReader.h
	$(CXX) $(CXXFLAGS) $(SRCDIR)/testxml.cpp -c -o $(OBJDIR)/testxml.o

$(OBJDIR)/XMLReader.o: $(SRCDIR)/XMLReader.cpp $(INCDIR)/XMLReader.h $(INCDIR)/XMLEntity.h
	$(CXX) $(CXXFLAGS) $(SRCDIR)/XMLReader.cpp -c -o $(OBJDIR)/XMLReader.o 

$(OBJDIR)/XMLWriter.o: $(SRCDIR)/XMLWriter.cpp $(INCDIR)/XMLWriter.h $(INCDIR)/XMLEntity.h
	$(CXX) $(CXXFLAGS) $(SRCDIR)/XMLWriter.cpp -c -o $(OBJDIR)/XMLWriter.o 

directories: $(BINDIR) $(OBJDIR) $(TESTBINDIR)

$(BINDIR):
	mkdir -p $(BINDIR)

$(OBJDIR):
	mkdir -p $(OBJDIR)

$(TESTBINDIR):
	mkdir -p $(TESTBINDIR)

clean:
	cd $(CSVLIBDIR) ; make clean ; cd ..	
	rm -f $(CSVLIBDIR)/Makefile
	rm -f $(OBJDIR)/*.o
	rm -f $(BINDIR)/*
	rm -f $(TESTBINDIR)/*