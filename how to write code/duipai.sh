i=0
while true
do
  i=$((i+1))
  echo "case $i"
  ./gen > in.txt
  ./brute < in.txt > ans.txt
  ./std < in.txt > out.txt
  diff ans.txt out.txt || break
done