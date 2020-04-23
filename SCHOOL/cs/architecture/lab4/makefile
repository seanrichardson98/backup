TARGET=listomatic

CC=cc
FLAGS=-std=c99 -Wall -Og

$(TARGET): listomatic.o
	$(CC) $(FLAGS) $^ -o $@

listomatic.o: listomatic.c
	$(CC) $(FLAGS) -c $<

clean:
	rm -f listomatic.o $(TARGET)
