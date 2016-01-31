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


int n_tables;
vector<string> tables;
map<string, vector<attr> > attributes;
map<string, attr> p_key;
map<string,map<string,vector<string> > > records;
map<string,vector<int> > max_width;
map<string,bool> valid;
int n_rel;
map<string,string> relations;		//key = foreign table,key, value = primary table,key(reference)
ifstream in_file;
string dummy;
// bool verifyType(string str,string type){
// 	if(type == "STRING") return regex_match(str,regex("[a-zA-Z0-9]+"));
// 	else if(type == "BOOLEAN") return regex_match(str,regex("true|TRUE|FALSE|false"));
// 	else if(type == "INTEGER") return regex_match(str,regex("(-)?[0-9]+"));
// 	else if(type == "REAL") return regex_match(str, regex("((-)?[0-9]+)|((-)?[0-9]+(\\\\.)[0-9]+)"));
// 	else if(type == "DATE") return regex_match(str, regex("(([0-9]{2})&&([^(00)]))/(([0-9]{2})&&([^(00)]))/(([0-9]{4})&&([^(0000)]))|(([0-9]{2})&&([^(00)]))"));
// 	else if(type == "TIME") return regex_match(str,regex("[0-9]{2}:[0-9]{2}:[0-9]{2} [ap]m"));
// 	else return false;
// }

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
	if(type == "STRING") return verifyString(str);
	else if(type == "BOOLEAN") return verifyBool(str);
	else if(type == "INTEGER") return verifyInt(str);
	else if(type == "REAL") return verifyReal(str);
	else if(type == "DATE") return verifyDate(str);
	else if(type == "TIME") return verifyTime(str);
	else if(type == "CURRENCY"){
		string str1 = "-";
		str1 += str;
		return verifyReal(str1);
	}
	else return false;
}

string createKey(vector<string> vec){
	string str = "";
	for(auto ele: vec) str += ele+",";
	return str;
}

void createTable(){
		string cur_table;
		in_file >> cur_table;
		valid[cur_table] = true;
		tables.push_back(cur_table);
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
				cout << "Invalid:\nmultiple primary keys in " << cur_table << " : " << p_key[cur_table].name << " and " << a_name << endl;
				// exit(0);
				valid[cur_table] = false;
			}
			// if(key_found == false){
			// 	cout << "no primary key in " << cur_table << endl;
			// }
		}
		int n_records;
		in_file >> n_records;
		getline(in_file,dummy,'\n');
		vector<string> p_key_val;
		for(int j=0;j<n_records;j++){
			vector<string> cur_record;
			string cur_a_val="";
			for(int k=0;k<n_attr-1;k++){
				getline(in_file,cur_a_val,',');
				if(!verifyType(cur_a_val,attributes[cur_table].at(k).type)){
					cout << cur_a_val <<"wrong type. Attribute type : " << attributes[cur_table].at(k).type << endl;
					// exit(0);
					valid[cur_table] = false;
				}
				if((p_key[cur_table].name == attributes[cur_table].at(k).name) && (find(p_key_val.begin(),p_key_val.end(),cur_a_val) != p_key_val.end())){
					cout << "duplicate primary key in records" << endl;
					valid[cur_table] = false;
				}
				else if(p_key[cur_table].name == attributes[cur_table].at(k).name){
						p_key_val.push_back(cur_a_val);
				}
				if(cur_a_val.length() > max_width[cur_table].at(k)) max_width[cur_table].at(k) = cur_a_val.length();
				cur_record.push_back(cur_a_val);
			}
			getline(in_file,cur_a_val,'\n');
			if(!verifyType(cur_a_val,attributes[cur_table].at(n_attr-1).type)){
				cout << cur_a_val <<"wrong type. Attribute type : " << attributes[cur_table].at(n_attr-1).type << endl;
				// exit(0);
				valid[cur_table] = false;
				if((p_key[cur_table].name == attributes[cur_table].at(n_attr-1).name) && (find(p_key_val.begin(),p_key_val.end(),cur_a_val) != p_key_val.end())){
					cout << "duplicate primary key in records" << endl;
					valid[cur_table] = false;
				}
				else if(p_key[cur_table].name == attributes[cur_table].at(n_attr-1).name) p_key_val.push_back(cur_a_val);
			}
			cur_record.push_back(cur_a_val);
			if(cur_a_val.length() > max_width[cur_table].at(n_attr-1)) max_width[cur_table].at(n_attr-1) = cur_a_val.length();
			if(p_key_val.size() > 0) records[cur_table][p_key_val.back()] = cur_record;
			else records[cur_table][createKey(cur_record)] = cur_record;
		}
}

void outputDB(){
	ofstream out_file("output.txt");
	for(auto cur_table: tables){
		out_file << cur_table << endl;
		if(valid[cur_table] == false){
			out_file << "Invalid"<<endl;
		}
		else{
			int i=0;
			for(auto cur_attr: attributes[cur_table]){
				out_file << setw(max_width[cur_table].at(i) + 5) << left << cur_attr.name;
				i++;
			}
			out_file << endl;
			i=0;
			for(auto cur_record: records[cur_table]){
				for(auto attr_val: cur_record.second){
					out_file << setw(max_width[cur_table].at(i) + 5) << left << attr_val;
					i++;
				}
				out_file << endl;
				i=0;
			}
		}
		out_file << endl;
	}
}

void checkRecords(){
	for(auto relation: relations){
		int d_pos = relation.first.find(',');
		string foreign_table = relation.first.substr(0,d_pos);
		string foreign_key = relation.first.substr(d_pos+1,relation.first.length()-d_pos-1);
		d_pos = relation.second.find(',');
		string primary_table = relation.second.substr(0,d_pos);
		string primary_key = relation.second.substr(d_pos+1,relation.second.length()-d_pos-1);
		int k_pos=0;
		for(auto att: attributes[foreign_table]){
			if(att.name == foreign_key) break;
			k_pos++;
		}
		for(auto foreign_record: records[foreign_table]){
			string f_val = foreign_record.second
		}
	}
}

void createRelations(){
	in_file >> n_rel;
	getline(in_file,dummy,'\n');
	for(int i=0;i<n_rel;i++){
		string primary_key;
		bool valid_rel = true;
		getline(in_file,primary_key,'(');
		getline(in_file,primary_key,'(');
		getline(in_file,primary_key,')');
		int d_pos = primary_key.find(',');
		string table1 = primary_key.substr(0,d_pos);
		string key = primary_key.substr(d_pos+1,primary_key.length()-d_pos-1);
		if(p_key[table1].name != key){
			cout << "Invalid relation:\n"<<key << " is not primary key of " << table1 << endl;
			valid_rel = false;
			// exit(0);
			// continue;
		}
		string foreign_key;
		getline(in_file,foreign_key,'(');
		getline(in_file,foreign_key,')');
		getline(in_file,dummy,'\n');
		d_pos = foreign_key.find(',');
		string table2 = foreign_key.substr(0,d_pos);
		key = foreign_key.substr(d_pos+1,foreign_key.length()-d_pos-1);
		bool key_p = false;
		if(!valid[table1]) valid[table2] = false;
		for(auto att: attributes[table2]){
			if(att.name == key){
				key_p=true;
				break;
			}
		}
		if(!key_p){
			cout << "Invalid relation:\n"<<key << " not present in " << table2 << endl;
			// exit(0);
			valid_rel = false;
		}
		if(!valid[table2]){
			cout << "Invalid relation: invalid table " << table2 << endl;
			// continue;
			valid_rel = false;
		}
		if(!valid[table1]){
			cout << "Invalid relation: invalid table " << table1 << endl;
			// continue;
			valid_rel = false;
		}
		if(valid_rel) relations[primary_key] = foreign_key;
	}
}

void createDB(string file){
	in_file.open(file);
	in_file >> n_tables;
	for(int i=0;i<n_tables;i++){
		createTable();
	}
	createRelations();
	outputDB();
}

int main(int argc, char** argv){
	if(argc < 2){
		cout << "Provide file name!" << endl;
		exit(0);
	}
	createDB(argv[1]);
	// cout << "\t\t\tRELATIONS :" << endl;
	// cout << "Table,foreign_key==>Table,primary_key"<<endl;
	// for(auto relation: relations){
	// 	cout << relation.first << "==>" << relation.second << endl;
	// }
	return 0;
}