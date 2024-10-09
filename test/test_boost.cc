#include <boost/version.hpp>
#include <boost/config.hpp>
#include <iostream>
#include <string>
#include "boost/lexical_cast.hpp" // 需要包含的头文件
 
void test_boost_version(){
    std::cout << BOOST_VERSION <<std::endl;
    std::cout << BOOST_LIB_VERSION<<std::endl;
    std::cout << BOOST_STDLIB <<std::endl;
    std::cout <<BOOST_PLATFORM <<std::endl;
    std::cout << BOOST_COMPILER<<std::endl;
}

void test_lexical_cast(){
    const char* p="32768";
    int i=0;
 	// 将字符串转化为整数
    i=boost::lexical_cast<int>(p); 
    std::cout << i << std::endl;
}


void test_find(){
	std::string prefix="xyz";
	std::string str="abcd";
	if(prefix.find_first_not_of(str)!=std::string::npos){
		std::cout << prefix << " is not in " << str << std::endl;
	}

	if(prefix.find_last_not_of(str)!=std::string::npos){
		std::cout << prefix << " is not in " << str << std::endl;
	}
}

int main(int argc,char** argv){
    test_boost_version();
    test_lexical_cast();
    test_find();
    return 0;
}