server:
	g++ -std=c++11 src/util.cpp client.cpp -o client && \
	g++ -std=c++11 src/util.cpp server.cpp src/Epoll.cpp src/InetAddress.cpp src/Socket.cpp src/Channel.cpp src/EventLoop.cpp src/Server.cpp -o server
clean:
	rm server && rm client
