all: eserver

eserver: EchoServer.cpp EchoClient.cpp
	c++ -o eserver EchoServer.cpp
	c++ -o eclient EchoClient.cpp

clean:
	rm -f *.o

fclean: clean
	rm -f eserver eclient

re: fclean all

.PHONY: all clean fclean re