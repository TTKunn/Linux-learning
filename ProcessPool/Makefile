BIN=process_pool
SRC=$(wildcard *.cc *.cpp)    
OBJ=$(SRC:.cc=.o)
cc=gcc
ccc=g++
RM=rm -f

$(BIN):$(OBJ)
	@$(ccc) $^ -o $@	
%.o:%.cc		
	@$(ccc) -c $< -g		
	@echo "编译 $< 成 $@"

.PHONY:clean
clean:
	@$(RM) $(OBJ) $(BIN)



#BIN=test1
#SRC=test1.c
#OBJ=test1.o
#cc=gcc
#RM=rm -f
#
#$(BIN):$(OBJ)
#	$(cc) $(OBJ) -o $(BIN)
#$(OBJ):$(SRC)
#	$(cc) -c $(SRC) -o $(OBJ)
#
#.PHONY:clean
#clean:
#	$(RM) $(BIN) $(OBJ)


#test1:test1.c
#	gcc test1.c -o test1
#
#.PHONY:clean
#clean:
#	rm -f test1;
