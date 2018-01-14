
cdecl:cdecl.c
	gcc -Wall -g -o $@ $<

.PHONY: clean 
clean:
	rm -rf cdecl



