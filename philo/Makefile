CC		=	gcc

SHELL	:= /bin/bash

RED		= \033[1;31m
BLUE	= \033[1;34m
CYAN	= \033[1;36m
ORANGE	= \033[1;38;5;214m
GREEN	= \033[1;32m
RESET	= \033[0m
UP1		= \033[1F
BEGIN	= \033[0G
SAVE	= \0337
RESTORE	= \0338
INSET	= $(BEGIN)$(GREEN)+ $(BLUE)

NAME		=	philosophers

SRCS		=	main.c philo.c init.c monitor.c utility.c

INC			=	-I ./include

OBJS		=	$(patsubst %.c, obj/%.o, $(SRCS))

COMPILED	=	0

DONE_OBJS	=	$(wildcard obj/*.o)

ifeq ($(words $(filter-out $(DONE_OBJS), $(OBJS))), 0)
TO_COMPILE	=	$(words $(OBJS))
else
TO_COMPILE	=	$(words $(filter-out $(DONE_OBJS), $(OBJS)))
endif

PERCENT		=	$(INSET)$(CYAN)$(shell echo \
				$$(($(COMPILED)*100/$(TO_COMPILE)))%%)

ifeq ($(DEBUG), 1)
CFLAGS	= -Wall -Wextra -Werror -fsanitize=thread -g
else
CFLAGS	= -Wall -Wextra -Werror -O3
endif

all: heading status comp

.PHONY: heading status comp clean fclean re rere debug

comp: $(NAME)

heading:
	@printf "$(CYAN)| $(ORANGE)Duco's $(NAME)\n$(RESET)"

obj/%.o: src/%.c
	@mkdir -p $(@D)
	@printf "$(PERCENT):$(RESET) "
	$(CC) $(CFLAGS) $(INC) -c $^ -o $@
	$(eval COMPILED=$(shell echo $$(($(COMPILED)+1))))

$(NAME): $(OBJS)
	@printf "$(INSET)"
	$(CC) $(CFLAGS) $(INC) $^ -o $@
	@printf "$(CYAN)| $(ORANGE)Created executable.\n$(RESET)"

depclean:
	@make -C libft/ fclean SILENT=1

clean: heading
	@rm -rf obj
	@printf "$(RED)- $(BLUE)Removed any object files.\n$(RESET)"

fclean: clean
	@rm -f $(NAME)
	@printf "$(RED)- $(BLUE)Removed executable file.\n$(RESET)"

re: fclean comp

rere: fclean depclean comp

debug:
	@make DEBUG=1
