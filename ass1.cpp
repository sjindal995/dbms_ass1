#include <iostream>
#include <string>
#include <regex>
#include <fstream>
#include <map>
#include <string.h>	
#include <iomanip>
#include <regex>
using namespace std;

struct attr{
	string name;
	string type;
};

bool verifyType(string str,string type){
	if(type == "STRING") return regex_match(str,regex("[a-zA-Z0-9]+"));
	else if(type == "BOOLEAN") return regex_match(str,regex("true|TRUE|FALSE|false"));
	else if(type == "INTEGER") return regex_match(str,regex("(-)?[0-9]+"));
	else if(type == "REAL") return regex_match(str, regex("((-)?[0-9]+)|((-)?[0-9]+(\\\\.)[0-9]+)"));
	else if(type == "DATE") return regex_match(str, regex("(([0-9]{2})&&([^(00)]))/(([0-9]{2})&&([^(00)]))/(([0-9]{4})&&([^(0000)]))|(([0-9]{2})&&([^(00)]))"));
	else if(type == "TIME") return regex_match(str,regex("[0-9]{2}:[0-9]{2}:[0-9]{2} [ap]m"));
	else return false;
}

int main(int argc, char** argv){
	if(argc < 2){
		cout << "Provide file name!" << endl;
		exit(0);
	}
	ifstream in_file(argv[1]);
	int n_tables;
	in_file >> n_tables;
	string tables[n_tables];
	map<string, vector<attr> > attributes;
	map<string, attr> p_key;
	map<string,vector<vector<string> > > records;
	string dummy;
	map<string,vector<int> > max_width;
	for(int i=0;i<n_tables;i++){
		string cur_table;
		in_file >> cur_table;
		tables[i] = cur_table;
		int n_attr;
		in_file >> n_attr;
		getline(in_file,dummy,'\n');
		bool key_found = false;
		for(int j=0;j<n_attr;j++) max_width[cur_table].push_back(0);
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
			if(a_name.length() > max_width[cur_table].at(j)) max_width[cur_table].at(j) = a_name.length();
			if((atoi(isKey.c_str()) == 1) && (!key_found)){
				key_found = true;
				p_key[cur_table] = cur_attr;
			}
			else if((atoi(isKey.c_str()) == 1) && (key_found)){
				cout << "multiple primary keys in " << cur_table << " : " << p_key[cur_table].name << " and " << a_name << endl;
				exit(0);
			}
			if(key_found == false){
				cout << "no primary key in " << cur_table << endl;
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
				if(!verifyType(cur_a_val,attributes[cur_table].at(k).type)){
					cout << cur_a_val <<"wrong type. Attribute type : " << attributes[cur_table].at(k).type << endl;
					exit(0);
				}
				if(cur_a_val.length() > max_width[cur_table].at(k)) max_width[cur_table].at(k) = cur_a_val.length();
				cur_record.push_back(cur_a_val);
			}
			getline(in_file,cur_a_val,'\n');
			cur_record.push_back(cur_a_val);
			if(cur_a_val.length() > max_width[cur_table].at(n_attr-1)) max_width[cur_table].at(n_attr-1) = cur_a_val.length();
			records[cur_table].push_back(cur_record);
		}
	}
	ofstream out_file("output.txt");
	for(auto cur_table: tables){
		out_file << cur_table << endl;
		int i=0;
		for(auto cur_attr: attributes[cur_table]){
			out_file << setw(max_width[cur_table].at(i) + 5) << left << cur_attr.name;
			i++;
		}
		out_file << endl;
		i=0;
		for(auto cur_record: records[cur_table]){
			for(auto attr_val: cur_record){
				out_file << setw(max_width[cur_table].at(i) + 5) << left << attr_val;
				i++;
			}
			out_file << endl;
			i=0;
		}
		out_file << endl;
	}
	int n_rel;
	in_file >> n_rel;
	getline(in_file,dummy,'\n');
	map<string,string> relations;		//key = foreign key, value = primary key(reference)
	for(int i=0;i<n_rel;i++){
		string primary_key;
		getline(in_file,primary_key,'(');
		getline(in_file,primary_key,'(');
		getline(in_file,primary_key,')');
		int d_pos = primary_key.find(',');
		string table = primary_key.substr(0,d_pos);
		string key = primary_key.substr(d_pos+1,primary_key.length()-d_pos-1);
		if(p_key[table].name != key){
			cout << key << " is not primary key of " << table << endl;
			exit(0);
		}
		string foreign_key;
		getline(in_file,foreign_key,'(');
		getline(in_file,foreign_key,')');
		getline(in_file,dummy,'\n');
		d_pos = foreign_key.find(',');
		table = foreign_key.substr(0,d_pos);
		key = foreign_key.substr(d_pos+1,foreign_key.length()-d_pos-1);
		bool key_p = false;
		for(auto att: attributes[table]){
			if(att.name == key){
				key_p=true;
				break;
			}
		}
		if(!key_p){
			cout << key << " not present in " << table << endl;
			exit(0);
		}
		relations[primary_key] = foreign_key;
	}
	// cout << "\t\t\tRELATIONS :" << endl;
	// cout << "Table,foreign_key==>Table,primary_key"<<endl;
	// for(auto relation: relations){
	// 	cout << relation.first << "==>" << relation.second << endl;
	// }
	return 0;
}