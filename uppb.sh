#!/bin/sh
#rm a.out
g++ uppb.cpp hexdump.c  log_logstores.pb-c.c -lcgicc -lz -llz4 -lprotobuf-c -I/usr/include -I/usr/local/include/protobuf-c/
#g++ uppb.cpp hexdump.c  -lcgicc -lz -I/usr/include
sudo cp a.out /var/www/cgi-bin/uppb.cgi
#sudo cp a.out /Library/WebServer/CGI-Executables/uppb.cgi
