import sys, os, glob

if len(sys.argv)<2:
	print("usage: python3",sys.argv[0],"typename");
	sys.exit();

gen_type = sys.argv[1];

files = glob.glob("vector_template*.c") + glob.glob("vector_template*.h");
for f in files:
	file_string = open(f).read();
	out_string = file_string.replace("TYPE", gen_type);
	print("Generating vector_" + gen_type + f[15:]);
	open("vector_" + gen_type + f[15:], "wt").write(out_string);

