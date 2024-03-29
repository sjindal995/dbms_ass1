#include <iostream>
#include <string>
#include <regex>

using namespace std;
using namespace regex_constants;
std::regex str_regex ("[a-z]");
// regex int_regex ("[+-]{,1}[0-9]+");
// regex bool_regex ("true|TRUE|FALSE|false");
// regex real_regex ("((-)?[0-9]+)|((-)?[0-9]+(.)[0-9]+)");
// regex date_regex ("(([0-9]{2})&&([^(00)]))/(([0-9]{2})&&([^(00)]))/(([0-9]{4})&&([^(0000)]))|(([0-9]{2})&&([^(00)]))");
// regex time_regex ("[0-9]{2}:[0-9]{2}:[0-9]{2} [ap]m");

bool verifyType(string str,string type){
	if(type == "str"){
		if(regex_match(str,(str_regex))) return true;
		else{
			cout << "regex not matched" << endl;
			return false;
		}
	}
	// else if(type == "bool") return regex_match(str,(bool_regex));
	// else if(type == "int") return regex_match(str,(int_regex));
	// else if(type == "real") return regex_match(str, (real_regex));
	// else if(type == "date") return regex_match(str, (date_regex));
	// else if(type == "time") return regex_match(str,(time_regex));
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

// #include <iostream>
// #include <algorithm>
// #include <string>
// #include <vector>

// using namespace std;

// int main(){
// 	vector<string> cur_attr_vec {"abc","def","abc","abc"};
// 	vector<string> sub_attr_vec {"abc","abc"};
// 	sort(cur_attr_vec.begin(),cur_attr_vec.end());
// 	sort(sub_attr_vec.begin(),sub_attr_vec.end());
// 	vector<string> v3;
// 	set_intersection(cur_attr_vec.begin(),cur_attr_vec.end(),sub_attr_vec.begin(),sub_attr_vec.end(),back_inserter(v3));
// 	cout << "size: " << v3.size() <<endl;
// 	for(string v: v3){
// 		cout << v << " ";
// 	}
// 	cout << endl;

// }
