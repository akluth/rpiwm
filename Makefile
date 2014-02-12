all: rpiwm

rpiwm:
	$(MAKE) -C src

clean:
	rm src/*.o

.PHONY: all rpiwm clean
