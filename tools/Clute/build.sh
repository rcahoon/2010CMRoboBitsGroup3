echo Building Shared Java Library...
cd ../SharedJavaLibrary
./build.sh
echo Building Clute...
cd ../Clute
find src -name *.java > files.txt
rm -rf bin
mkdir bin
javac -classpath ../SharedJavaLibrary/SharedJavaLibrary.jar -sourcepath src -d bin @files.txt
rm files.txt

