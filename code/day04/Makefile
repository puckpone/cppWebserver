server:
	g++ -std=c++11 util.cpp client.cpp -o client && \
	g++ -std=c++11 util.cpp server.cpp Epoll.cpp InetAddress.cpp Socket.cpp -o server
clean:
	rm server && rm client