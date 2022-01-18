#ifndef LEXICAL_ANALYSER_H_  /* Include guard */
#define LEXICAL_ANALYSER_H_

int foo(int x);  /* An example function declaration */


extern char token[];
bool isValidDelimiter(char ch) ;
bool isOparith(char ch);
bool isSpecialChar(char ch);
bool isOprel(char ch);
bool isvalidIdentifier(char* str);
bool isValidKeyword(char* str);
bool isValidInteger(char* str);
bool isRealNumber(char* str);
char* subString(char* str, int left, int right);
void detectTokens(char* str);





#endif // FOO_H_
