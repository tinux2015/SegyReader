# SegyReader
SEG-Y Reading in C++

使用方法（Method）：

（1）放置目录(Put the file in your dir)

将文件SegyReader.h和SegyReader.cpp放置在所要调用的工程相同目录下；

（2）头文件包含(Include the header file)

#include"SegyReader.h"

（3）建立实例(Instance)

SegyReader segy("test_file_name.sgy");

（4）调用成员函数(Call the member function)

例如：

segy.outputTD2Txt（“output_file_name”,start_trace,endt_trace）;
