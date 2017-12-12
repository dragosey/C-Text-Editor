build: 
	gcc -g main.c -o editor
clean:
	rm editor
run:
	./editor date.in operatii.in output.out
checker:
	./checker.sh

