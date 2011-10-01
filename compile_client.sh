cd MultiTaches
g++ -c *.cpp
ar rcs libmultitaches.a *.o
cd ../
g++ TP-Client/TP-Client/*.cpp TP-Client/TP-Client/*.c ./MultiTaches/libmultitaches.a -lpthread -o client -I ./MultiTaches/
rm MultiTaches/Pool.o
rm MultiTaches/stdafx.o
rm MultiTaches/Worker.o
rm MultiTaches/libmultitaches.a
