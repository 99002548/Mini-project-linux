all :all.out
all.out : mutex.o 
         gcc $^ -o $@ -lgtest -lgtest_main lpthread
mutex.o : mutex.c
         gcc $< -c
clean:
         rm -rf *.o *.out 
