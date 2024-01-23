all: bin/WareHouse

bin/WareHouse: bin/WareHouse.o bin/BaseAction.o bin/customer.o bin/order.o bin/volunteer.o
	g++ bin/WareHouse.o bin/BaseAction.o bin/customer.o bin/order.o bin/volunteer.o -o bin/WareHouse

bin/BaseAction.o: src/BaseAction.cpp
	g++ -g -Wall -Weffc++ -std=c++11 -c -Iinclude -o bin/BaseAction.o src/BaseAction.cpp

bin/WareHouse.o: src/main.cpp  # Adjusted the path to main.cpp
	g++ -g -Wall -Weffc++ -std=c++11 -c -Iinclude src/main.cpp -o bin/WareHouse.o

bin/customer.o: src/customer.cpp
	g++ -g -Wall -Weffc++ -std=c++11 -c -Iinclude -o bin/customer.o src/customer.cpp

bin/order.o: src/order.cpp
	g++ -g -Wall -Weffc++ -std=c++11 -c -Iinclude -o bin/order.o src/order.cpp

bin/volunteer.o: src/volunteer.cpp
	g++ -g -Wall -Weffc++ -std=c++11 -c -Iinclude -o bin/volunteer.o src/volunteer.cpp

.PHONY: clean
clean:
	rm -f bin/*
