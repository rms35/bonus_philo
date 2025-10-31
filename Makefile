NAME = philo_bonus
SRC = philo_bonus.c\
	utils_bonus.c\
	parsing_bonus.c\
	timeft_bonus.c\
	simulation.c\
	
OBJ = $(SRC:.c=.o)
CFLAGS += -g -O1 -pthread #-Wall -Wextra -Werror -fsanitize=thread

all: $(NAME)

$(NAME): $(OBJ)
	cc $(CFLAGS) $(OBJ) -o $(NAME)

%.o: %.c
	cc $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)