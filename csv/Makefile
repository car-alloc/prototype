NAME=test-csv
SOURCE= \
	test-csv.cpp \
	csvstream.cpp

HEADERS=*.hpp

OBJS=$(SOURCE:.cpp=.o)

DEFINES=

CXXFLAGS= -O3 \
	-std=c++11 \
	-W \
	-Wall \
	-Wextra \
	$(DEFINES) \
	-I.

#	-I$(CURDIR)

LDFLAGS=

all:   $(NAME)

$(NAME):  $(OBJS)
	g++ -o $(NAME) $(OBJS) $(LDFLAGS)

clean:
	rm -v $(OBJS)

# counts "real" lines of code
count:
	@echo full: $(shell cat $(SOURCE) $(HEADERS) | wc -l)
	@echo useful: $(shell \
	grep -v \
	-e "^[ ]*//" \
	-e "^[ ]*$$" \
	-e "^[ ]*{[ ]*$$" \
	-e "^[ ]*}[ ]*$$" \
	$(SOURCE) $(HEADERS) | wc -l )
