#include <iostream>
#include <string>

using namespace std;

bool verifyString(string str){
	for(auto c: str){
		if((int(c) < 48) || (int(c) > 122)) return false;
	}
	return true;
}

bool verifyBool(string str){
	if(str == "true" || str == "TRUE" || str == "false" || str == "FALSE") return true;
	return false;
}

bool verifyInt(string str){
	if(str[0] == '+' || str[0] == '-') str = str.substr(1,str.length()-1);
	int i=0;
	for(auto c: str){
		if(int(c) < 48 || int(c) > 57) return false;
		i++;
	}
	return true;
}

bool verifyReal(string str){
	if(str[0] == '+' || str[0] == '-') str = str.substr(1,str.length()-1);
	bool dot_found = false;
	for(int i=0;i<str.length();i++){
		char c = str[i];
		if((c == '.') && (!dot_found) && (i != 0) && (i != str.length()-1)){
			dot_found = true;
			continue;
		}
		else if(((c == '.') && (dot_found || i == 0 || i == str.length()-1)) || (int(c) < 48 || int(c) > 57)) return false;
	}
	return true;	
}

bool verifyDate(string str){
	if(str[2] == '/' && str[5] == '/'){
		str[2] = '0';
		str[5] = '0';
	}
	else return false;
	if((str.length() != 8) && (str.length() != 10)) return false;
	string str1 = "-";
	str1 += str;
	return verifyInt(str1);
}

bool verifyTime(string str){
	if(str[2] == ':' && str[5] == ':'){
		str[2] = '0';
		str[5] = '0';
	}
	if((str[str.length()-3] == ' ') && (str[str.length()-2] == 'a' || str[str.length()-2] == 'p') && (str[str.length()-1] == 'm')) str = str.substr(0,str.length()-3);
	else if((str[str.length()-3] != ' ') && (str[str.length()-2] == 'a' || str[str.length()-2] == 'p') && (str[str.length()-1] == 'm')) str = str.substr(0,str.length()-2);
	if(str.length() != 8) return false;
	string str1 = "-";
	str1 += str;
	return verifyInt(str1);
}

bool verifyType(string str, string type){
	if(type == "str") return verifyString(str);
	else if(type == "bool") return verifyBool(str);
	else if(type == "int") return verifyInt(str);
	else if(type == "real") return verifyReal(str);
	else if(type == "date") return verifyDate(str);
	else if(type == "time") return verifyTime(str);
	else return false;
}

int main(){
	string str;
	string type;
	while(1){
		cin >> str;
		cin >> type;
		cout << verifyType(str,type) << endl;
	}
}