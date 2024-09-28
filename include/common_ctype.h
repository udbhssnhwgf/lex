#ifndef _COMMON_CTYPE_H_
#define _COMMON_CTYPE_H_
#include <stdio.h>
#include <string.h>
typedef unsigned char           CF_U08;
typedef unsigned short          CF_U16;
typedef unsigned int            CF_U32;

typedef signed char             CF_S08;
typedef short                   CF_S16;
typedef int                     CF_S32;

typedef unsigned long           CF_UL;
typedef signed long             CF_SL;

typedef float                   CF_F32;
typedef double                  CF_F64;

#ifndef _M_IX86
    typedef unsigned long long  CF_U64;
    typedef long long           CF_S64;
#else
    typedef unsigned __int64    CF_U64;
    typedef __int64             CF_S64;
#endif

typedef char                    CF_CHAR;

typedef unsigned int            CF_HANDLE;



#define LOG_BUF_LEN 256
class LogBuffer
{
public:
    static char* Get()
    {
        static __thread char data[LOG_BUF_LEN];
        return data;
    }

    static char* End()
    {
        return LogBuffer::Get() + strlen(LogBuffer::Get());
    }
};

#define PRINT_STDOUT(tag, fmt, ...) {snprintf(LogBuffer::Get(), LOG_BUF_LEN, tag " : [%s %s %d] : " fmt, __FILE__, __FUNCTION__, __LINE__, ##__VA_ARGS__); \
                                     fprintf(stdout, "%s", LogBuffer::Get());}

#define ERROR_PRT(fmt, ...) PRINT_STDOUT("[ERROR]", fmt, ##__VA_ARGS__)
#define INFO_PRT(fmt, ...)  PRINT_STDOUT("[INFO ]", fmt, ##__VA_ARGS__)
#define WARN_PRT(fmt, ...)  PRINT_STDOUT("[WARN ]", fmt, ##__VA_ARGS__)
#define DEBUG_PRT(fmt, ...) PRINT_STDOUT("[DEBUG]", fmt, ##__VA_ARGS__)

#ifdef PRINT_DEBUG
#define TIME_RECORD(name) unsigned long name = BdtdGetSysTime()
#define PRINTF(fmt, ...)    PRINT_STDOUT("[PRINT]", fmt, ##__VA_ARGS__)
#else
#define TIME_RECORD(name)
#define PRINTF(fmt, ...)
#endif


#define CHECK_ERROR(err, fmt, ...) do{          \
    if (err)                                    \
    {                                           \
        ERROR_PRT(fmt, ##__VA_ARGS__);          \
        return err;                             \
    }                                           \
}while(0)

#define CHECK_GOTO(ret, label, fmt, ...)        \
  do                                            \
  {                                             \
    if (ret)                                    \
    {                                           \
      ERROR_PRT(fmt, ##__VA_ARGS__);            \
      goto label;                               \
    }                                           \
  } while (0)




#define ALIGN_UP(num, align) (((num) + (align) - 1) & ~((align) - 1))
#define ALIGN_UP2(num) ALIGN_UP(num, 2)
#define ALIGN_UP16(num) ALIGN_UP(num, 16)
#define ALIGN_UP128(num) ALIGN_UP(num, 128)

#define ALIGN_DOWN(val, align) ((val / align) * align)

#define FREE(p) if(p) {free(p); p = NULL;}
#define DELETE(p) if(p) {delete p; p = NULL;}
#endif
