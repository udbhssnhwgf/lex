#ifndef _LEX_H_
#define _LEX_H_
#include <stdio.h>
#include <string.h>
#include "common_ctype.h"
#include <vector>
#define MAX_TOKEN_LEN 255

class Token{
public:
    Token(){}
    ~Token(){}

    int Init()
    {
        m_flag = 0;
        m_line = 0;
        m_len  = 0;
        m_pos  = 0;
        return 0;
    }

    int Set(const char* data, int len, int line, int pos)
    {
        memcpy(m_lexeme, data, len);
        m_lexeme[len] = '\0';
        m_len = len;
        m_line = line;
        m_pos = pos;
        m_flag = 1;
        return 0;
    }

    int SetType(int type)
    {
        m_type = type;
        return 0;
    }

    int GetFlag()
    {
        return m_flag;
    }
    char* GetLexStr()
    {
        return m_lexeme;
    }
private:
    int m_type;
    int m_flag;
    int m_line;
    int m_pos;
    int m_len;
    char m_lexeme[MAX_TOKEN_LEN + 1];
};

#define LEX_EOF -3


class TextStream{
public:
    TextStream(const char* data, long len);
    ~TextStream();
    int IsEnd();
    int SkipSpace();
public:
    int m_line; // current line
    int m_pos;  // current pos
    const char* m_data;
    const char* m_end;
    const char* m_p;
    long m_data_len;
};

#define LEX_ERROR -1
#define LEX_COMPLETE 512
enum StringType{
    INIT,
    OTHER,
    ALPHA,
};

class ParseToken{
public:
    ParseToken();
    virtual int GetType(const char c) = 0;
    virtual int Complete(Token* token, const char* start) = 0;
    virtual int GetState(const char c) = 0;

    int GetNextToken(TextStream* text, Token* token);
    int CopyFromText(TextStream* text);
    int CopyToText(TextStream* text);
    int IsEnd();
protected:
    int m_state;
    int m_line;
    int m_pos;
    const char* m_p;
    const char* m_end;
};

class ParseString:public ParseToken{
public:
    ParseString();
    ~ParseString();

    int GetType(const char c) override;
    int Complete(Token* token, const char* start) override;
    int GetState(const char c) override;
private:
                    // INIT         OTHER           ALPHA
    int convert[3][3] = {{INIT,     LEX_ERROR,      ALPHA,}, 
                        {LEX_ERROR, LEX_ERROR,      LEX_ERROR,}, 
                        {LEX_ERROR, LEX_COMPLETE,   ALPHA,}};
};


class Lex{
public:
    Lex(){}
    ~Lex(){}
    int SetTextStream(TextStream* text)
    {
        m_text = text;
        return 0;
    }

    int GetToken(Token* token)
    {
        int ret;
        ret = m_parse_str.GetNextToken(m_text, token);
        if (LEX_ERROR == ret)
        {
            m_text->m_p++;
            m_text->m_pos++;
        }
        return ret;
    }
private:
    TextStream* m_text;
    // std::vector<ParseToken*> m_parse;
    ParseString m_parse_str;
};

#endif