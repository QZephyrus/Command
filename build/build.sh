#!/bin/bash
g++ ../src/command.cpp ../src/Compression_Trace.cpp ../src/DataBase.cpp -ocommand -I/usr/include/python2.7 -lpython2.7 -lboost_date_time `mysql_config --cflags --libs` -Wall