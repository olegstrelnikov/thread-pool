/*
 * main.cpp
 *
 *  Created on: Mar 9, 2017
 *      Author: ois
 */


#include <thread-pool.hpp>

#include <iostream>

template <std::size_t N> void f() {
	std::cout << "f<" << N << ">()" << "\n";
}

int main() {
	ThreadPool threadPool(10);
	threadPool.addTask(f<1>);
	threadPool.addTask(f<2>);
	threadPool.addTask(f<3>);
	threadPool.addTask(f<4>);
	threadPool.addTask(f<5>);
	threadPool.addTask(f<6>);
	threadPool.addTask(f<7>);
	threadPool.addTask(f<8>);
	threadPool.addTask(f<9>);
	threadPool.addTask(f<10>);
	threadPool.addTask(f<11>);
	threadPool.addTask(f<12>);
	threadPool.addTask(f<13>);
	threadPool.addTask(f<14>);
	threadPool.addTask(f<15>);
	threadPool.addTask(f<16>);
	return 0;
}
