
loop = int(input())
monster = loop-1
ans = 0;
i = 1;
while True:
  ans += 1
  if (i-1)%loop == monster or i == monster or (i+1) % loop == monster:
    break
  i += 2
  i = i % loop
  monster += 1
  monster = monster % loop
print (ans)
