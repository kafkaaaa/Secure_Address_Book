/*
<Base64 인코딩 과정>

#1. 평문을 ASCII 코드로 변환
#2. ASCII 코드를 2진수로 변환
#3. 2진수를 6bit 단위로 나눔  (**여기서 마지막이 6bit가 되지 않으면 0으로 padding)
#4. 6bit씩 나눠진 것을 Base64 표에서 찾아 바꿈 (**6bit씩 남는 부분은 '=' 로 채워 넣음)

**사용 이유?
    ASCII는 7bit(128개) 와 1bit(Parity bit = 오류 검출) 를 사용하는데,
    Parity bit를 처리하는 방식이 시스템 마다 달라서 
    데이터를 전달할 때 시스템 환경에 따라 문제가 될 수 있다.
    Base64는 ASCII 코드 중에서 제어문자와 일부 특수문자를 제외한 64개의 *안전한 문자만 사용한다.
                            (*안전한 문자 = 문자 코드에 영향을 받지 않는 ASCII 문자를 의미)

-단점: 원본 데이터 보다 약 1.3배 늘어남 (8비트씩 나누던 것을 6비트로 나눔)
-장점: 안정적으로 binary to text 변환 가능, 플랫폼간 호환성 좋음 

-결론: Base64는 Binary Data를 공통 ASCII 만을 사용하여 데이터를 정확하게 전송하기 위한 인코딩 방법이다.
*/


#include "Base64.h"

const char base64code_ascii[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

const unsigned char asciicode_base64code[256] =
{
 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
 // 0x2b +, 0x2f /
 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 62, 0xff, 0xff, 0xff, 63, 
 // 0x30 : 0,1,2,...
 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
 // 0x40: ＠,A,B,C,...
 0xff, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 
 //0x50: P,Q,R,S,...
 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 0xff, 0xff, 0xff, 0xff, 0xff, 
 // 0x60 : ｀,a,b,c,...
 0xff, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 
 // 0x70 : p,q,r,s,...
 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 0xff, 0xff, 0xff, 0xff, 0xff,
 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff
};


int text_to_base64_size(const char *text)
{
 int len = strlen(text);
 return ((len + 2) / 3 * 4);
}



int base64_encoder(char *in, int in_size, char *out, int out_size)
{
 int i = 0;
 int o = 0;

 if (!in || !out) return 0;
 if (text_to_base64_size(in) > out_size) return 0;

 while (i < in_size - 2 && o+4 <= out_size)
 {
 out[o++] = base64code_ascii[(in[i] >> 2) & 0x3F];
 out[o++] = base64code_ascii[((in[i] & 0x3) << 4) | ((int)(in[i + 1] & 0xF0) >> 4)];
 out[o++] = base64code_ascii[((in[i + 1] & 0xF) << 2) | ((int)(in[i + 2] & 0xC0) >> 6)];
 out[o++] = base64code_ascii[in[i + 2] & 0x3F];
 i += 3;
 }

 if (i < in_size)
 {
 out[o++] = base64code_ascii[(in[i] >> 2) & 0x3F];
 if (i == (in_size - 1))
 {
 out[o++] = base64code_ascii[((in[i] & 0x3) << 4)];
 out[o++] = '='; // padding
 }
 else
 {
 out[o++] = base64code_ascii[((in[i] & 0x3) << 4) | ((int)(in[i + 1] & 0xF0) >> 4)];
 out[o++] = base64code_ascii[((in[i + 1] & 0xF) << 2)];
 }

 out[o++] = '='; // padding
 }

 return o;
}



int base64code_size(const char *code, int len)
{
 int real_code_len = 0;
 if (!code)
 return 0;

 while (real_code_len < len && asciicode_base64code[code[real_code_len]] != 0xff)
 {
 real_code_len++;
 }

 return real_code_len;
}

int base64_to_text_size(const char *code,int len)
{
 return ((base64code_size(code,len) + 3) / 4) * 3;
}




int base64_decoder(const char *in, int in_size, char *out, int out_size)
{
 int i = 0;
 int o = 0;
 int real_code_len = 0;

 if (!in || !out) return 0;
 if (out_size < base64_to_text_size(in, in_size)) return 0;

 real_code_len = base64code_size(in, in_size);


 while (i + 4 < real_code_len)
 {
 out[o++] = (char)(asciicode_base64code[in[i]] << 2 | asciicode_base64code[in[i+1]] >> 4);
 out[o++] = (char)(asciicode_base64code[in[i + 1]] << 4 | asciicode_base64code[in[i + 2]] >> 2);
 out[o++] = (char)(asciicode_base64code[in[i + 2]] << 6 | asciicode_base64code[in[i + 3]]);
 i += 4;
 }

 if (real_code_len - i > 1)
 {
 out[o++] = (char)(asciicode_base64code[in[i]] << 2 | asciicode_base64code[in[i + 1]] >> 4);
 }

 if (real_code_len - i > 2)
 {
 out[o++] = (char)(asciicode_base64code[in[i + 1]] << 4 | asciicode_base64code[in[i + 2]] >> 2);
 }

 if (real_code_len - i > 3)
 {
 out[o++] = (char)(asciicode_base64code[in[i + 2]] << 6 | asciicode_base64code[in[i + 3]]);
 }

 return o;
}







// int base64_encode(char *text, int numBytes, char **encodedText)
// {
//   uint8_t input[3]  = {0,0,0};
//   uint8_t output[4] = {0,0,0,0};
//   int   index, i, j, size;
//   char *p, *plen;

//   plen           = text + numBytes - 1;
//   size           = (4 * (numBytes / 3)) + (numBytes % 3? 4 : 0) + 1;
//   (*encodedText) = malloc(size);
//   j              = 0;

//     for  (i = 0, p = text;p <= plen; i++, p++) {
//         index = i % 3;
//         input[index] = *p;

//         if (index == 2 || p == plen) {
//             output[0] = ((input[0] & 0xFC) >> 2);
//             output[1] = ((input[0] & 0x3) << 4) | ((input[1] & 0xF0) >> 4);
//             output[2] = ((input[1] & 0xF) << 2) | ((input[2] & 0xC0) >> 6);
//             output[3] = (input[2] & 0x3F);

//             (*encodedText)[j++] = MimeBase64[output[0]];
//             (*encodedText)[j++] = MimeBase64[output[1]];
//             (*encodedText)[j++] = index == 0? '=' : MimeBase64[output[2]];
//             (*encodedText)[j++] = index <  2? '=' : MimeBase64[output[3]];

//             input[0] = input[1] = input[2] = 0;
//         }
//     }

//     (*encodedText)[j] = '\0';

//     return 0;
// }


// int base64_decode(char *text, uint8_t *dst, int numBytes)
// {
//   const char* cp;
//   int space_idx = 0, phase;
//   int d, prev_d = 0;
//   uint8_t c;

//     space_idx = 0;
//     phase = 0;

//     for ( cp = text; *cp != '\0'; ++cp ) {
//         d = DecodeMimeBase64[(int) *cp];
//         if ( d != -1 ) {
//             switch ( phase ) {
//                 case 0:
//                     ++phase;
//                     break;
//                 case 1:
//                     c = ( ( prev_d << 2 ) | ( ( d & 0x30 ) >> 4 ) );
//                     if ( space_idx < numBytes )
//                         dst[space_idx++] = c;
//                     ++phase;
//                     break;
//                 case 2:
//                     c = ( ( ( prev_d & 0xf ) << 4 ) | ( ( d & 0x3c ) >> 2 ) );
//                     if ( space_idx < numBytes )
//                         dst[space_idx++] = c;
//                     ++phase;
//                     break;
//                 case 3:
//                     c = ( ( ( prev_d & 0x03 ) << 6 ) | d );
//                     if ( space_idx < numBytes )
//                         dst[space_idx++] = c;
//                     phase = 0;
//                     break;
//             }
//             prev_d = d;
//         }
//     }

//     return space_idx;

// }

