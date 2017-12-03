import math
import time
n = int(input())
angleincs = [0] + [math.radians(360.0/x) for x in range(1, 9)]

pts = []
for i in range(n):
  xy = [int(x) for x in raw_input().split(" ")]
  pts.append((xy[0], xy[1]))

def getpts(base, num):
  pts = []
  inc = 360.0 / num
  angleinc = angleincs[num]
  for i in range(1, num+1):
    angle = angleinc * i
    o = base*math.sin(angle)
    a = base*math.cos(angle)
    pts.append((a, o))
  return pts

def vec(ptA, ptB):
  return (ptB[0]-ptA[0], ptB[1]-ptA[1]) 

def crs(ptA, ptB, ptC):
  ca = vec(ptC, ptA)
  cb = vec(ptC, ptB)
  return ca[0]*cb[1]-ca[1]*cb[0]

def true_if_outside(det):
#  print(det)
  return det > 0

def true_if_inside(det):
#  print(det)
  return det < 0

def containsall(guesspts, pts):
  for i in range(len(guesspts)):
    for pt in pts:
      if true_if_inside(crs(guesspts[i-1], guesspts[i], pt)):
        return False
  return True

def containsnone(guesspts, pts):
  for pt in pts:
    inside = True
    for i in range(len(guesspts)):
      if true_if_inside(crs(guesspts[i-1], guesspts[i], pt)):
        inside = False
    if inside:
      return False
  return True

def getarea(pts):
  area = 0.0
  for i in range(len(pts)):
    area += (pts[i-1][0] * pts[i][1])-(pts[i-1][1]*pts[i][0])
  return area

def getans(numverts):
  bestout = 0
  hi = 1000000001.0
  lo = 0.0
  while hi > lo:
    mid = (hi+lo)/2.0
    guesspts = getpts(mid, numverts)
    if containsall(guesspts, pts):
      bestout = mid
      hi = mid - 0.0000001
    else:
      lo = mid + 0.0000001
  
  bestin = 0
  hi = 1000000001.0
  lo = 0.0
  while hi > lo:
    mid = (hi+lo)/2.0
    guesspts = getpts(mid, numverts)
    if containsnone(guesspts, pts):
      bestin = mid
      lo = mid + 0.0000001
    else:
      hi = mid - 0.0000001
  
  return getarea(getpts(bestin, numverts)) / getarea(
     getpts(bestout, numverts))
  
best = 3
bestans = getans(3)
for i in range(4, 9):
  ans = getans(i)
  if ans > bestans:
    best = i
    bestans = ans
print best, round(bestans, 6)
