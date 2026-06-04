CC     = gcc
CFLAGS = -Wall -Wextra -lm
SRC    = src/test.c src/hash.c src/linked_list.c src/queue.c src/stack.c \
         src/logistic.c src/data_to_sorted_csv.c src/sorted_csv_to_dll.c
TEST_SRC = src/test_units.c src/hash.c src/linked_list.c src/queue.c \
           src/stack.c src/data_to_sorted_csv.c src/sorted_csv_to_dll.c
OUT    = bin/hostel
TEST_OUT = bin/test_units

all: bin $(OUT)

bin:
	mkdir -p bin

$(OUT): $(SRC)
	$(CC) $(SRC) -I include -o $(OUT) $(CFLAGS)

test: bin $(TEST_OUT)
	./$(TEST_OUT)

$(TEST_OUT): $(TEST_SRC)
	$(CC) $(TEST_SRC) -I include -o $(TEST_OUT) $(CFLAGS)

clean:
	rm -f bin/hostel bin/hostel.exe bin/test_units bin/test_units.exe
	rm -f src/sorted_students.csv allocated_rooms.csv allocations.dat

.PHONY: all test clean