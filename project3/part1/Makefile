#
# Makefile
#

all: webSearch

webSearch: webSearch.c indexPage crawler
	gcc -Wall -o webSearch indexPage.o crawler.o webSearch.c

indexPage:
	gcc -Wall -c indexPage.c -o indexPage.o

crawler:
	gcc -Wall -c crawler.c -o crawler.o
