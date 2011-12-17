#!/usr/bin/python
import compiler
import marshal
import sys
import os.path

if len(sys.argv) < 2:
    print "usage:", sys.argv[0], "input_py output_cpp"
    sys.exit(1)

source = sys.argv[1]
target = sys.argv[2]
basename = os.path.basename(source)
arrayname  = basename.lower().replace(".","_")
code = compiler.compile(open(source).read(), basename, "exec")
f = open("ce.dump", "wb")
marshal.dump(code, f)
f.close()

f = open("ce.dump", "rb")
data = f.read()
f.close()
os.remove("ce.dump")

f = open(target, "w")

br = 16
n = len(data) - 1
f.write("#include <cstdio>\n")
f.write("\n")
f.write("unsigned char "+arrayname+"_wrapper[] = {")
for i in range(0, n):
	if i % br == 0:
		f.write("\n    ")
	f.write(hex(ord(data[i])))
	if (i < n-1):
		f.write(", ")

f.write("\n};\n")
f.write("size_t "+arrayname+"_wrapper_size = " + str(len(data)) + ";\n")
f.close()

