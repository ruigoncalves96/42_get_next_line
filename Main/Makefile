CC = cc
CCFLAGS = -Wall -Wextra -Werror
RM = rm -f

NAME = GNL

SRC = $(addprefix ../, get_next_line.c get_next_line_utils.c) GNL_Main/get_next_line_main.c
OBJ = $(SRC:%.c=%.o)

BONUS_SRC = $(addprefix ../, get_next_line_bonus.c get_next_line_utils_bonus.c) GNL_BONUS_Main/get_next_line_main_bonus.c
BONUS_OBJ = $(BONUS_SRC:%.c=%.o)

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(CCFLAGS) $^ -o $@

%.o: %.c
	$(CC) $(CCFLAGS) -c $< -o $@

bonus: $(BONUS_OBJ)
	$(CC) $(CCFLAGS) $^ -o $(NAME)

clean:
	$(RM) $(OBJ) $(BONUS_OBJ)

fclean: clean
	$(RM) $(NAME)

re: fclean all
