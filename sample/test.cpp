#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "lex.h"
#include "utils.h"

int main(int argc, char* argv[])
{
    int ret;
    const char* filename = "../utils/utils.h";
    char*data;
    long size;
    printf("filename = [%s]\n", filename);
    ret = ReadTxtFile(filename, &data, &size);
    CHECK_ERROR(ret, "open file failed\n");
    printf("data = [%s]\n", data);

    TextStream* text = new TextStream(data, size);
    Lex* lex = new Lex();
    lex->SetTextStream(text);
    Token token;

    while(1)
    {
        ret = lex->GetToken(&token);
        if (token.GetFlag()) printf("%s\n", token.GetLexStr());
        if (ret == LEX_EOF) break;
    }

    DELETE(lex);
    DELETE(text);
    return 0;
}