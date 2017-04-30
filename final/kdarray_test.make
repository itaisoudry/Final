CC = gcc
OBJS = sp_kd_array_unit_test.o KDArray.o
EXEC = sp_kd_array_unit_test
TESTS_DIR = ./unit_tests
COMP_FLAG = -std=c99 -Wall -Wextra -Werror -pedantic-errors

$(EXEC): $(OBJS)
	$(CC) $(OBJS) -o $@

	
sp_kd_array_unit_test.o: $(TESTS_DIR)/sp_kd_array_unit_test.c $(TESTS_DIR)/sp_kd_array_unit_test.h KDTree.h KDArray.h
	$(CC) $(COMP_FLAG) -c $(TESTS_DIR)/$*.c

KDArray.o: KDArray.c KDArray.h SPLogger.h Utils/LoggerMessages.h \
 Utils/AllocationHandler.h Utils/ResponseCodes.h SPPoint.h

KDTree.o: KDTree.c KDTree.h SPPoint.h KDArray.h SPLogger.h \
 Utils/LoggerMessages.h Utils/AllocationHandler.h Utils/ResponseCodes.h \
 SPBPriorityQueue.h

	$(CC) $(COMP_FLAG) -c $*.c
clean:
	rm -f $(OBJS) $(EXEC)