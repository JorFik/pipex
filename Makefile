# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: JFikents <JFikents@student.42Heilbronn.de> +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/10/18 21:57:25 by JFikents          #+#    #+#              #
#    Updated: 2023/12/05 18:56:43 by JFikents         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

RM = rm -rf
CC = cc
CALLMAKE = make -C
CFLAGS = -Wall -Wextra -Werror -Iheaders/ -Ilibft/h_files
ADD = -fsanitize=address -g
OBJ+ = $(C_FILES:.c=.o) $(BONUS_FILES:.c=.o)
OBJ = $(C_FILES:.c=.o)
LIBS_D = libft/
DEBUGGER = debugger/

NAME = pipex
MAIN =
TEST =
A_FILE = libft.a
C_FILES = src/pipex.c src/pipex_utils.c src/pipex_exec_cmd.c
BONUS_FILES =

.PHONY: clean fclean re all c

all: $(NAME)

bonus: $(OBJ+) a_files
	@$(CC) -o $@ $(OBJ+)

$(NAME) : $(OBJ) a_files
	@echo "	Compiling $(NAME)..."
	@$(CC) -o $@ $< *.a
	@make clean

a_files: $(LIBS_D)
	@for dir in $(LIBS_D); do \
		echo "	Compiling $$dir..."; \
		$(CALLMAKE) $$dir; \
	done

%.o : %.c 
	@echo "	Compiling $@..."
	@$(CC) $(CFLAGS) -c -o $@ $<

clean:
	@echo "	Cleanig traces..."
	@echo "	Ereasing Files .o"
	@$(RM) $(OBJ+)
	@echo "	Ereasing Files .a"
	@$(RM) *.a

fclean: clean
	@echo "	Ereasing $(NAME)..."
	@$(RM) $(NAME)

re: fclean all

debug: c a_files
	@$(CC) $(ADD) $(CFLAGS) $(H_FILE) $(C_FILES) $(MAIN) *.a
	@mv a.out.dSYM $(DEBUGGER)
	@mv a.out $(DEBUGGER)
	@mv *.a $(DEBUGGER)
	@make fclean

test: c
	@$(CC) $(ADD) $(CFLAGS) $(H_FILE) $(TEST)
	@mv a.out.dSYM $(DEBUGGER)
	@mv a.out $(DEBUGGER)
	@mv *.gch $(DEBUGGER)
	@make fclean

c: fclean
	@$(RM) $(DEBUGGER)* 
	@$(RM) *.out *.dSYM *.gch 