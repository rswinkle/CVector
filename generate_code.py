import sys, os

if len(sys.argv)<2:
	print("usage: python3",sys.argv[0],"typename");
	sys.exit();

gen_type = sys.argv[1];

hfile_string = open("vector_template.h").read();
cfile_string = open("vector_template.c").read();

h_out = hfile_string.replace('TYPE', gen_type);
c_out = cfile_string.replace('TYPE', gen_type);

open("vector_"+gen_type+".h", "wt").write(h_out);
open("vector_"+gen_type+".c", "wt").write(c_out);


