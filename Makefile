NAME = philo_bonus
HEL = philo_hel
SRC = philo_bonus.c\
	utils_bonus.c\
	parsing_bonus.c\
	simulation.c\
	simulation2.c\
	free.c\
	time_bonus.c\
	
OBJ = $(SRC:.c=.o)
OBJ_H = $(SRC:.c=.o)
CFLAGS += -g -O3 -pthread #-Wall -Wextra -Werror -fsanitize=thread
HFLAGS += -g -pthread #-Wall -Wextra -Werror -fsanitize=thread
FLAGS = $(CFLAGS)
ifeq ($(MAKECMDGOALS),hell)
FLAGS = $(HFLAGS)
endif

all: $(NAME)

hel: $(HEL)

$(NAME): $(OBJ)
	cc $(FLAGS) $(OBJ) -o $(NAME)

$(HEL): $(OBJ_H)
	cc $(FLAGS) $(OBJ_H) -o $(HEL)

%.o: %.c
	cc $(FLAGS) -c $< -o $@

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re: fclean all