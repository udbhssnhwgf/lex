#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "lex.h"

TextStream::TextStream(const char* data, long len)
{
    m_data = data;
    m_end = data + len;
    m_data_len = len;
    m_p = data;
    m_line = 1;
    m_pos = 1;
}

TextStream::~TextStream(){}

int TextStream::IsEnd()
{
    return m_p >= m_end;
}

int TextStream::SkipSpace()
{
    while(1)
    {
        if (IsEnd())
        {
            return LEX_EOF;
        }

        char c = *m_p;
        if (' ' == c || '\t' == c || '\r' == c || '\n' == c)
        {
            m_p++;
            m_pos++;
            if ('\n' == c)
            {
                m_line++;
                m_pos = 1;
            }
        }
        else
        {
            break;
        }
    }
    return 0;
}

ParseToken::ParseToken()
{
    m_state = INIT;
}

int ParseToken::GetNextToken(TextStream* text, Token* token) 
{
    int ret = 0;
    token->Init();
    ret = text->SkipSpace();
    CHECK_ERROR(ret, "skip space\n");
    CopyFromText(text);
    const char* start_ptr = NULL;
    m_state = INIT;
    while(1)
    {
        if (IsEnd())
        {
            ret = LEX_EOF;
            if (start_ptr){
                token->Set(start_ptr, m_p - start_ptr, m_line, m_pos);
                m_state = LEX_COMPLETE;
            }
            break;
        }

        char c = *m_p;
        // printf("c = [%c] state = [%d]\n", c, m_state);
        m_state = GetState(c);
        if (LEX_ERROR == m_state)
        {
            ret = LEX_ERROR;
            break;
        }
        else 
        {
            if (NULL == start_ptr)
            {
                start_ptr = m_p;
            }
        }

        if (LEX_COMPLETE == m_state)
        {
            Complete(token, start_ptr);
            break;
        }
        m_p++;
        m_pos++;
    }

    if (LEX_COMPLETE == m_state)
    {
        CopyToText(text);
    }
    return ret;
}

int ParseToken::CopyFromText(TextStream* text)
{
    m_line = text->m_line;
    m_pos = text->m_pos;
    m_p = text->m_p;
    m_end = text->m_end;
    return 0;
}

int ParseToken::CopyToText(TextStream* text)
{
    text->m_line = m_line;
    text->m_pos  = m_pos;
    text->m_p    = m_p;
    text->m_end  = m_end;
    return 0;
}

int ParseToken::IsEnd()
{
    return m_p >= m_end;
}



ParseString::ParseString()
{

}
ParseString::~ParseString()
{
}

int ParseString::GetType(const char c)
{
    if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || '_' == c)
    {
        return ALPHA;
    }
    else
    {
        return OTHER;
    }
    return 0;
}

int ParseString::Complete(Token* token, const char* start)
{
    int ret;
    token->Set(start, m_p - start, m_line, m_pos);
    return 0;
}

int ParseString::GetState(const char c)
{
    int type = GetType(c);
    return convert[m_state][type];
}
