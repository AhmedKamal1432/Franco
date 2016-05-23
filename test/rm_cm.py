import sys
if __name__ == '__main__':
    f = sys.argv[1]
    with open (f, "r") as myfile:
        lines=myfile.readlines()

    for l  in lines:
	if(l[0:2] == "//"):
	    continue;
	print(l),
