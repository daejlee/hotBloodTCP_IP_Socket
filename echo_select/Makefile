all: es_server

es_server: EchoServer.cpp EchoClient.cpp
	c++ -g -o es_server EchoServer.cpp
	c++ -g -o eclient EchoClient.cpp

clean:
	rm -rf *.o *.dSYM

fclean: clean
	rm -f es_server eclient

re: fclean all

.PHONY: all clean fclean re