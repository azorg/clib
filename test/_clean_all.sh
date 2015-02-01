#! /bin/sh

m="Makefile"
p=`pwd`

for d in *
do
  test -d "$d" || continue
  cd "$d"
  if [ -f "$m" ]
  then
    echo "clean in $d"
    make clean  
  fi
  cd "$p"
done
