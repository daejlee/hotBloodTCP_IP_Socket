all: eserver

eserver: EchoServer.cpp EchoClient.cpp
	c++ -g -o eserver EchoServer.cpp
	c++ -g -o eclient EchoClient.cpp

clean:
	rm -rf *.o *.dSYM

fclean: clean
	rm -f eserver eclient

re: fclean all

.PHONY: all clean fclean re