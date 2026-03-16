#include<iostream>
#include<bitset>

int main(){
	
	int j = 4;
	std::cout << std::bitset<8>(j) << std::endl;
	j |= (1 << 0) | (1 << 5);

	std::cout << std::bitset<8>(j) << std::endl;
}
