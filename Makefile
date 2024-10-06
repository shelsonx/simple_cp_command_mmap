CC = gcc

CFLAGS = -Wall -Werror -Wextra -g3

MYCP = mycp

all: getbook $(MYCP)

$(MYCP): my_cp.c
	$(CC) $(CFLAGS) -o $@ $<

getbook: 
	wget -O book.txt https://www.gutenberg.org/cache/epub/6527/pg6527.txt

run: all
	./mycp book.txt book_bkp.txt
	ls
	stat book.txt book_bkp.txt

clean:
	rm -rf book*.txt

fclean: clean
	rm -rf $(MYCP)

re: fclean all 

.PHONY: clean fclean re getbook run
