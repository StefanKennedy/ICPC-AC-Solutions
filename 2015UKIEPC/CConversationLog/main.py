import sys

n = input()

d = {}
said = {}
uset = set()

for line in sys.stdin:
    line = line.strip()
    u = line.split(" ")[0]
    uset.add(u)
    for toke in line.split(" ")[1:]:
        if toke in d:
            d[toke]+=1
        else:
            d[toke]=1
        if toke not in said:
            said[toke] = set()
        said[toke].add(u)
y = sorted([(d[x]*-1, x) for x in said if len(said[x]) == len(uset)])
for x in y:
    print(x[1])
if len(y) == 0:
    print("ALL CLEAR")
