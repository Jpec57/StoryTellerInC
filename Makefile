IDIR=includes
ODIR=obj
SDIR=src
CC=gcc
CFLAGS=-I $(IDIR)
_DEPS = birthday.h
_OBJ = main.o  
DEPS=$(patsubst %,$(IDIR)/%,$(_DEPS))
OBJ=$(patsubst %,$(ODIR)/%,$(_OBJ))
NAME=birthday

all: $(NAME)

$(ODIR)/%.o: $(SDIR)/%.c $(DEPS)
	@$(CC) -c -o $@ $< $(CFLAGS)

$(NAME): $(OBJ)
	@$(CC) -o $@ $^ $(CFLAGS)

clean:
	@rm -rf $(OBJ)

fclean: clean
	@rm -rf $(NAME)

re: fclean all

.PHONY: all clean fclean re
