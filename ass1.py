#!/usr/bin/python
import sys
import re
file_name = (sys.argv)[1]
in_f = open(file_name)
n_tables = int((in_f.readline())[:-1])
tables = [];
attributes
for index in range(0,n_tables):
	cur_table = (in_f.readline())[:-1]
	n_attr = int((in_f.readline())[:-1])
	for attr_index in range(0,n_attr):
		line = (in_f.readline())[:-1]
		line_li = re.split(",",line)
	n_records = int((in_f.readline())[:-1])
	for rec_index in range(0,n_records):
