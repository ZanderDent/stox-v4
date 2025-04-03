QT = $(shell pkg-config --cflags --libs Qt6Widgets Qt6Charts)
SRC = main.cpp viewer.cpp controller.cpp data_fetcher.cpp feature_engineering.cpp ml_model.cpp moc_viewer.cpp
CXX = clang++
CXXFLAGS = -std=c++17 -Wall -Wno-deprecated-declarations
LDFLAGS = -L/opt/homebrew/opt/lapack/lib -L/opt/homebrew/opt/openblas/lib
LIBS = -L/opt/homebrew/lib -lboost_serialization -lboost_program_options -lcurl -lopenblas -llapack
# Find Qt installation path and moc
QTDIR = /opt/homebrew/Cellar/qt/6.8.2_1
MOC = $(QTDIR)/bin/moc

all: stockTraderApp

moc_viewer.cpp: viewer.h
	$(MOC) viewer.h -o moc_viewer.cpp

stockTraderApp: $(SRC)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $(SRC) -o $@ $(QT) $(LIBS)

clean:
	rm -f stockTraderApp moc_viewer.cpp