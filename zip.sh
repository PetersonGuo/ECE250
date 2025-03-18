if [ -z "$1" ]; then
  echo "project # arg required"
  exit 1
fi

tar -czvf p9guo_$1.tar.gz data_structures src include p9guo_design_$1.txt passcode.txt Makefile