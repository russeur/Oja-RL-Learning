#include "HebianSelection.hpp"
#include <fstream>
#include <iostream>



int main(int argc, char*argv[]){
	
	for (int i = 1; i<argc; i++){
		std::cout<<i<<": "<<argv[i]<<"\n";
	}
	std::ofstream ofs;
	ofs.open("test.bin", std::ofstream::out | std::ofstream::trunc | std::ofstream::binary);
	int* nums = new int[100];
	int num ;
	for (int i = 0; i<100; i++){
		*(nums+i) = i;
	}
	
	ofs.write(reinterpret_cast<const char*>(nums), sizeof(num));
	for (int i = 0; i<100; i++){
		*(nums+i) = 0;
	}
	ofs.close();
	std::ifstream ifs;
	ifs.open("test.bin",   std::ofstream::in | std::ofstream::binary);
	ifs.read(reinterpret_cast<char*>(nums), sizeof(num)*100);
	for (int i = 0; i<100; i++){
		
		std::cout<<*(nums+i)<<"\n";
	}
	ifs.close();
	delete [] nums;
	
	std::cout<<"good job: "<<"\n";
	
}







