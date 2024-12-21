#include <vector>
#include <iostream>
#include <string>
#include <iomanip>
#include <fstream>
#include<cmath>
#include<cstdlib>
using namespace std;
string intToStr(int n); 

string intToStr(int n){
	string str;
	int digit=1;
	vector<int>ivs;
	if(n==0){
		return "0";
	}
	while(n!=0){
		int r=n%static_cast<int>(pow(10,digit));
		ivs.push_back(r);
		n=n/pow(10,digit);
	}
	for(int i=ivs.size()-1;i>=0;i--){
		switch(ivs[i]){
			case 0:
				str=str+"0";
				break;
			case 1:
				str=str+"1";
				break;
			case 2:
				str=str+"2";
				break;
			case 3:
				str=str+"3";
				break;
		    case 4:
				str=str+"4";
				break;
			case 5:
				str=str+"5";
				break;
		    case 6:
				str=str+"6";
				break;
			case 7:
				str=str+"7";
				break;
			case 8:
				str=str+"8";
				break;
			case 9:
				str=str+"9";
				break;
			
		}
	}
	
	return str;
}
