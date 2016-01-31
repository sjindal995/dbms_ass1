#include <iostream>
#include <string>
#include <regex>
#include <fstream>
#include <map>
#include <string.h>	
using namespace std;

struct attr{
	string name;
	string type;
};

int main(int argc, char** argv){
	ifstream in_file(argv[1]);
	int n_tables;
	in_file >> n_tables;
	string tables[n_tables];
	map<string, vector<attr> > attributes;
	map<string, attr> p_key;
	map<string,vector<vector<string> > > records;
	string dummy;
	for(int i=0;i<n_tables;i++){
		string cur_table;
		in_file >> cur_table;
		tables[i] = cur_table;
		int n_attr;
		in_file >> n_attr;
		getline(in_file,dummy,'\n');
		bool key_found = false;
		for(int j=0;j<n_attr;j++){
			string a_name;
			getline(in_file,a_name,',');
			// a_name = a_name.substr(2,a_name.length()-2);
			string a_type;
			getline(in_file,a_type,',');
			string isKey;
			// getline(in_file,isKey,')');
			getline(in_file,isKey,'\n');
			// cout << "aname:" << a_name << ":::::" << "a_type:" << a_type << "::::::isKey:" <<isKey<<   endl;
			attr cur_attr;
			cur_attr.name = a_name;
			cur_attr.type = a_type;
			attributes[cur_table].push_back(cur_attr);
			if((atoi(isKey.c_str()) == 1) && (!key_found)){
				key_found = true;
				p_key[cur_table] = cur_attr;
			}
		}
		int n_records;
		in_file >> n_records;
		getline(in_file,dummy,'\n');
		for(int j=0;j<n_records;j++){
			vector<string> cur_record;
			string cur_a_val="";
			for(int k=0;k<n_attr-1;k++){
				getline(in_file,cur_a_val,',');
				cur_record.push_back(cur_a_val);
			}
			getline(in_file,cur_a_val,'\n');
			cur_record.push_back(cur_a_val);
			records[cur_table].push_back(cur_record);
		}
	}
	// for(auto cur_table: tables){
	// 	cout << cur_table << endl;
	// 	for(auto cur_attr: attributes[cur_table]){
	// 		cout << cur_attr.name << "\t\t";
	// 	}
	// 	cout << endl;
	// 	for(auto cur_record: records[cur_table]){
	// 		for(auto attr_val: cur_record){
	// 			cout << attr_val << "\t\t";
	// 		}
	// 		cout << endl;
	// 	}
	// 	cout << endl;
	// }
	int n_rel;
	in_file >> key_rel;
	getline(in_file,dummy,'\n');
	for(int i=0;i<n_rel;i++){
		
	}
}