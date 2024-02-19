CC = gcc
<<<<<<< HEAD
CFLAGS  += -Wall -Werror -Wextra -g3 -Wno-unused-parameter -Wno-unused-function
=======
CFLAGS  += -Wall -Werror -Wextra -g3 -Wunused-parameter # -E for preprocessor output
>>>>>>> da0b89e3e94f52bb4e5356dec0825eb6aa2f7615
SANITIZE = -fsanitize=address
MAIN = otp
SRCS = $(wildcard *.c) munit/munit.c 
INCLUDES := $(INCLUDES) munit/munit.h
OBJS = $(SRCS:.c=.o)

ALL: $(MAIN)

$(MAIN): $(OBJS)
	$(CC) $(CFLAGS) $(SANITIZE) -o $@ $(LINKERFLAG) $^


debugger: $(SRCS)
	$(CC) $(CFLAGS) $(SANITIZE) -g -o $@ $(LINKERFLAG) $(SRCS)

clean:
	$(RM) $(OBJS)

fclean: clean
	$(RM) $(MAIN) $(DEBUG)

re: fclean all