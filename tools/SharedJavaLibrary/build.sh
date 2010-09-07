rm -rf bin
rm -rf sharedJavaLibrary.jar
#echo Building Java files...
mkdir bin
find src -name *.java > files.txt
javac -sourcepath src -d bin @files.txt
rm files.txt
#echo Making a .jar library...
cd bin
jar cf ../SharedJavaLibrary.jar *
cd ..

