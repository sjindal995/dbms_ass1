#include <iostream>
#include <string>
#include <regex>

using namespace std;

string str_regex = "[a-zA-Z0-9]+";
string int_regex = "[\\\\+\\\\-]{,1}[0-9]+";
string bool_regex = "true|TRUE|FALSE|false";
string real_regex = "((-)?[0-9]+)|((-)?[0-9]+(\\\\.)[0-9]+)";
string date_regex = "(([0-9]{2})&&([^(00)]))/(([0-9]{2})&&([^(00)]))/(([0-9]{4})&&([^(0000)]))|(([0-9]{2})&&([^(00)]))";
string time_regex = "[0-9]{2}:[0-9]{2}:[0-9]{2} [ap]m";

bool verifyType(string str,string type){
	if(type == "str") return regex_match(str,regex(str_regex));
	else if(type == "bool") return regex_match(str,regex(bool_regex));
	else if(type == "int") return regex_match(str,regex(int_regex));
	else if(type == "real") return regex_match(str, regex(real_regex));
	else if(type == "date") return regex_match(str, regex(date_regex));
	else if(type == "time") return regex_match(str,regex(time_regex));
	else return false;
}

int main(){
	while(1){
		string s;
		string type;
		cin >> s;
		cin >> type;
		cout << verifyType(s,type) << endl;;
	}
}