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

NAME		=	philo

SRCS		=	main.c philo.c philo_utils.c init.c monitor.c utility.c cleanup.c start_threads.c

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

$(NAME): $(OBJS)
	@printf "$(INSET)"
	$(CC) $(CFLAGS) $(INC) $^ -o $@
	@printf "$(GREEN)+ $(ORANGE)Created executable.\n$(RESET)"

all: $(NAME)

.PHONY: clean fclean re rere debug

obj/%.o: src/%.c
	@mkdir -p $(@D)
	@printf "$(PERCENT):$(RESET) "
	$(CC) $(CFLAGS) $(INC) -c $^ -o $@
	$(eval COMPILED=$(shell echo $$(($(COMPILED)+1))))

depclean:
	@make -C libft/ fclean SILENT=1

clean:
	@printf "$(RED)- $(BLUE)"
	rm -rf obj
	@printf "$(RESET)"

fclean: clean
	@printf "$(RED)- $(BLUE)"
	rm -f $(NAME)
	@printf "$(RESET)"

re: fclean all

rere: fclean depclean all

debug:
	@make DEBUG=1
