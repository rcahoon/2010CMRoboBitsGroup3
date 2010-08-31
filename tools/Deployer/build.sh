find src -name *.java > files.txt
rm -rf bin
mkdir bin
javac -sourcepath src -d bin @files.txt
rm files.txt

