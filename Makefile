NAME = webserv

CC = c++

C_FILES = main.cpp  lunch_server.cpp clients_obj.cpp parssingOfHeader.cpp parssingOfBody.cpp

H_FILES =   lunch_server.hpp clients_obj.hpp  parssingOfHeader.hpp parssingOfBody.hpp

C_FLAGS =-std=c++98  #-g -fsanitize=address #-Wall -Wextra -Werror -std=c++98

O_FILES = $(C_FILES:.cpp=.o)


%.o: %.cpp  ${H_FILES}
	@${CC} ${C_FLAGS} -c $< -o $@

all: ${NAME}

${NAME}: ${O_FILES} ${H_FILES}
	@${CC} ${C_FLAGS} ${O_FILES} -o ${NAME}

clean:
	@rm -f ${O_FILES}

fclean: clean
	@rm -f ${NAME}

re: fclean all