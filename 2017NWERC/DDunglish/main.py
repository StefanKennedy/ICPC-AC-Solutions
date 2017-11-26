#include <stdio.h>

dutchToEng = {};
wordToCount = {};

numWords = int(input());
origWords = input()
for word in origWords.split(" "):
  if word not in wordToCount:
    wordToCount[word] = 0;
  wordToCount[word]+=1;

wordToFound = {}
wordToCorrect = {}
numTrans = int(input())
for i in range(numTrans):
  tokens = input().split(" ")
  dutch = tokens[0]
  if dutch not in wordToFound:
    wordToFound[dutch] = 0;
  wordToFound[dutch]+=1
  if tokens[2] == "correct":
    if dutch not in wordToCorrect:
      wordToCorrect[dutch] = 0
    wordToCorrect[dutch] += 1
  dutchToEng[dutch] = tokens[1]

totalAns = 1;
for word in wordToFound.keys():
  if word in wordToCount:
    totalAns *= wordToFound[word] ** wordToCount[word]

# All words wrong still says correct translation

correctAns = 1;
for word in wordToCorrect.keys():
  if word in wordToCount:
    correctAns *= wordToCorrect[word] ** wordToCount[word]

for word in origWords.split(" "):
  if word not in wordToCorrect:
    correctAns = 0;

for word in origWords.split(" "):
  if word not in wordToFound:
    totalAns = 0;

if totalAns == 1:
  translated = []
  correct = True
  for word in origWords.split(" "):
    translated.append(dutchToEng[word]);
    if (word not in wordToCorrect):
      correct = False
  print (" ".join(translated))
  print ("correct" if correct else "incorrect")
else:
  print(correctAns, "correct")
  print(totalAns - correctAns, "incorrect")
