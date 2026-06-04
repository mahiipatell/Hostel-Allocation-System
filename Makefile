CC = gcc
CFLAGS = -Wall -Wextra -lm
SRC = src/test.c src/hash.c src/linked_list.c src/queue.c src/stack.c \
      src/logistic.c src/data_to_sorted_csv.c src/sorted_csv_to_dll.c
OUT = bin/hostel

all: bin $(OUT)

bin:
	mkdir -p bin

$(OUT): $(SRC)
	$(CC) $(SRC) -I include -o $(OUT) $(CFLAGS)

clean:
	rm -f bin/hostel bin/hostel.exe src/sorted_students.csv allocated_rooms.csv

.PHONY: all clean