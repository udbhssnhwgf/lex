

#include <stdio.h>
#include <stdlib.h>
#include "common_ctype.h"
long get_file_size(FILE *stream)
{
    long file_size = -1;
    long cur_offset = ftell(stream);	// 获取当前偏移位置
    if (cur_offset == -1) {
        ERROR_PRT("ftell failed\n");
        return -1;
    }
    if (fseek(stream, 0, SEEK_END) != 0) {	// 移动文件指针到文件末尾
        ERROR_PRT("fseek failed\n");
        return -1;
    }
    file_size = ftell(stream);	// 获取此时偏移值，即文件大小
    if (file_size == -1) {
        ERROR_PRT("ftell failed\n");
    }
    if (fseek(stream, cur_offset, SEEK_SET) != 0) {	// 将文件指针恢复初始位置
        ERROR_PRT("fseek failed\n");
        return -1;
    }
    return file_size;
}

int ReadTxtFile(const char* name, char** data, long* size){
    *data = NULL;
    *size = 0;
    FILE* fp = fopen(name, "r");
    if (!fp)
    {
        ERROR_PRT("can't open file = [%s]\n", name);
        return -1;
    }
    long filesize = get_file_size(fp);
    char *buffer = (char*)malloc(filesize + 1);
    *size = fread(buffer, 1, filesize, fp);
    *data = buffer;
    fclose(fp);
    return 0;
}
