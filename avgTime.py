count = 0
S = 0;
try:
  while True:
     a = input()
     count += 1;
     S = S + int(a)
except:
  print S/count
