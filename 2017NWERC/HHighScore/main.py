n = int(input())

def answer(a, b, c):
  return a**2+b**2+c**2 + 7 * min(a, b, c);

def requiredtoinc(a, b, c, level):
  return max(0, level - a) + max(0, level - b) + max(0, level - c)

for i in range(n):
  vals = [int(x) for x in input().split(" ")]
  a = vals[0]
  b = vals[1]
  c = vals[2]
  d = vals[3]
  if (d == 0):
    print(answer(a, b, c))
  else:
    sortd = sorted([a, b, c])
    bestanswer = answer(sortd[0], sortd[1], sortd[2]+d)

    low = 0
    hi = 1000000000
    while low < hi:
      l=(low+hi)//2
      required = requiredtoinc(a, b, c, l)
      if (required > d):
        hi = l
        continue
      dleft = d-required
      leveled = (max(l, sortd[0]), max(l, sortd[1]), max(l, sortd[2])+dleft)
      bestanswer = max(bestanswer,  answer(leveled[0], leveled[1], leveled[2]))
      low = l + 1 
    print(bestanswer)
