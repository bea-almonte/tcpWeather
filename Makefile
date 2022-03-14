CXX = g++
CXXFLAGS = -g -std=c++11 -Wall -Werror=return-type  -Werror=uninitialized -pthread # --coverage
# Do not allow compiling if a non-void function is missing a return statement
# Do not allow compiling if a variable is used without being initialized

temp: temp.o user.o
	$(CXX) $(CXXFLAGS) -o $@ $^
server: server.o tcpServer.o user.o location.o
	$(CXX) $(CXXFLAGS) -o $@ $^

client: client.o tcpClient.o
	$(CXX) $(CXXFLAGS) -o $@ $^

clean:
	rm -rf *.dSYM
	$(RM) *.o *.gc* server client