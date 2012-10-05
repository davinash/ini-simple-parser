P_NAME      :=  tini
P_CXX_SRCS  :=  $(wildcard *.cpp)
P_CXX_OBJS  :=  ${P_CXX_SRCS:.cpp=.o}
P_OBJS      :=  $(P_CXX_OBJS) 
CPPFLAGS    +=  $(foreach includedir,$(P_INCLUDE_DIRS),-I$(includedir))
LDFLAGS     +=  $(foreach librarydir,$(P_LIBRARY_DIRS),-L$(librarydir))
LDFLAGS     +=  $(foreach library,$(P_LIBRARIES),-l$(library))
CXX         :=  g++ -g
CC          :=  g++ -g

.PHONY:         all clean
all:            $(P_NAME)
$(P_NAME):      $(P_OBJS)
	$(CC) $(CCFLAGS) $(P_CXX_OBJS) -o $(P_NAME) $(LDFLAGS)
	ctags -R .
clean:
	@- $(RM) $(P_NAME)
	@- $(RM) $(P_OBJS)
	@- $(RM) core*
	@- $(RM) tags


