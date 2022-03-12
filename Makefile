CXX = g++
CXXFLAGS = -g -std=c++11 -Wall -Werror=return-type  -Werror=uninitialized -pthread # --coverage
# Do not allow compiling if a non-void function is missing a return statement
# Do not allow compiling if a variable is used without being initialized

TCPserver: TCPserver.o user.o
	$(CXX) $(CXXFLAGS) -o $@ $^
server: server.o
	$(CXX) $(CXXFLAGS) -o $@ $^

client: client.o
	$(CXX) $(CXXFLAGS) -o $@ $^

clean:
	rm -rf *.dSYM
	$(RM) *.o *.gc* server client TCPserver