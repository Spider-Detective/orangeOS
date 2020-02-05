#include "type.h"
#include "const.h"
#include "protect.h"
#include "string.h"
#include "proc.h"
#include "tty.h"
#include "console.h"
#include "global.h"
#include "keyboard.h"
#include "proto.h"

/*
 * Convert the given int to a string in base, and store the string in *ps
 */
PRIVATE char* i2a(int val, int base, char** ps) {
    int m = val % base;
    int q = val / base;
    if (q) {     // convert until the last digit
        i2a(q, base, ps);
    }
    *(*ps)++ = (m < 10) ? (m + '0') : (m - 10 + 'A');

    return *ps;
}

/*
 * move all args to the position declared in fmt into buf
 * e.g. fmt: "My char is: %c, length is %013d"
 *      args: 's', 12
 *      The buf after calling vsprintf(): "My char is: s, length is 0000000000012"
 * return value: the length in buf that has been written
 * 
 * We will call the system call printx(buf) to show the input formatted string
 */
int vsprintf(char* buf, const char* fmt, va_list args) {
    char* p;
    va_list p_next_arg = args;
    int m;

    char inner_buf[STR_DEFAULT_LEN];
    char cs;
    int align_nr;

    for (p = buf; *fmt; fmt++) {
        if (*fmt != '%') {
            *p++ = *fmt;           // simply store the char into p
            continue;
        } else {
            align_nr = 0;
        }
        fmt++;

        if (*fmt == '%') {         // % followed by %, will simply write out a '%'
            *p++ = *fmt;
            continue;
        } else if (*fmt == '0') {  // 0 followed by %, proceed with '0'
            cs = '0';
            fmt++;
        } else {                   // all other case set to space
            cs = ' ';
        }
        // e.g. printf ("Preceding with zeros: %012d \n", 1977);
        // '%012d' -> align_nr == 12, cs == '0'
        while ((unsigned char)(*fmt) >= '0' && ((unsigned char)(*fmt) <= '9')) {
            align_nr *= 10;
            align_nr += *fmt - '0';
            fmt++;
        }

        char* q = inner_buf;
        memset(q, 0, sizeof(inner_buf));

        switch(*fmt) {
            case 'c':           // char
                *q++ = *((char*)p_next_arg);
                p_next_arg += 4;
                break;
            case 'x':           // unsigned hex
                m = *((int*)p_next_arg);
                i2a(m, 16, &q);
                p_next_arg += 4;
                break;
            case 'd':           // signed decimal
                m = *((int*)p_next_arg);
                if (m < 0) {
                    m = m * (-1);
                    *q++ = '-';
                }
                i2a(m, 10, &q);
                p_next_arg += 4;
                break;
            case 's':
                strcpy(q, (*((char**)p_next_arg)));
                q += strlen(*((char**)p_next_arg));
                p_next_arg += 4;
                break;
            default:
                break;
        }

        int k;
        // if the align_nr is not met, fill up p with cs
        for (k = 0; k < ((align_nr > strlen(inner_buf)) ? (align_nr - strlen(inner_buf)) : 0); k++) {
            *p++ = cs;
        }
        q = inner_buf;       // write the inner_buf into p, or the buf, after extracting the arg
        while (*q) {
            *p++ = *q++;
        }
    }

    *p = 0;

    return (p - buf);
}

int sprintf(char* buf, const char* fmt, ...) {
    va_list arg = (va_list)((char*)(&fmt) + 4);    // skip fmt to get the argument list
    return vsprintf(buf, fmt, arg);
}