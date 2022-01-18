
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "lexical_analyser.h"

char token[200] = "";


bool isValidDelimiter(char ch) {
   if (ch == ' ' || ch == '+' || ch == '-' || ch == '*' ||
   ch == '/' || ch == ',' || ch == ';' || ch == '>' ||
   ch == '<' || ch == '(' || ch == ')' || ch == ':' || ch == '='||
   ch == '[' || ch == ']' || ch == '{' || ch == '}')
   return (true);
   return (false);
}

bool isOparith(char ch){
   if (ch == '+' || ch == '-' || ch == '*' ||
   ch == '/' )
   return (true);
   return (false);
}

bool isSpecialChar(char ch){
   if (ch == '(' || ch == ')' || ch == ';' || ch == ',' )
   return (true);
   return (false);
}

bool isOprel(char ch){
   if (ch == '>' || ch == '<')
   return (true);
   return (false);
}

bool isvalidIdentifier(char* str){
   if (str[0] == '0' || str[0] == '1' || str[0] == '2' ||
   str[0] == '3' || str[0] == '4' || str[0] == '5' ||
   str[0] == '6' || str[0] == '7' || str[0] == '8' ||
   str[0] == '9' || isValidDelimiter(str[0]) == true)
   return (false);
   return (true);
}

bool isValidKeyword(char* str) {
   if (!strcmp(str, "si") || !strcmp(str, "Si") || !strcmp(str, "sinon") || !strcmp(str, "Sinon") || !strcmp(str, "alors") || !strcmp(str, "Alors")|| !strcmp(str, "finsi") || !strcmp(str, "Finsi") || !strcmp(str, "FinSi")
   || !strcmp(str, "non") || !strcmp(str, "et") || !strcmp(str, "ou") || !strcmp(str, ":=")
   || !strcmp(str, "entier") || !strcmp(str, "reel") || !strcmp(str, "var")
   || !strcmp(str, "Program") || !strcmp(str, "Debut") || !strcmp(str, "Fin")
   || !strcmp(str, "while") || !strcmp(str, "do") ||    !strcmp(str, "break") || !strcmp(str, "continue") || !strcmp(str, "int")
   || !strcmp(str, "double") || !strcmp(str, "float") || !strcmp(str, "return") || !strcmp(str,    "char") || !strcmp(str, "case") || !strcmp(str, "char")
   || !strcmp(str, "sizeof") || !strcmp(str, "long") || !strcmp(str, "short") || !strcmp(str, "typedef") || !strcmp(str, "switch") || !strcmp(str, "unsigned")
   || !strcmp(str, "void") || !strcmp(str, "static") || !strcmp(str, "struct") || !strcmp(str, "goto"))
   return (true);
   return (false);
}

bool isValidInteger(char* str) {
   int i, len = strlen(str);
   if (len == 0)
   return (false);
   for (i = 0; i < len; i++) {
      if (str[i] != '0' && str[i] != '1' && str[i] != '2'&& str[i] != '3' && str[i] != '4' && str[i] != '5'
      && str[i] != '6' && str[i] != '7' && str[i] != '8' && str[i] != '9' || (str[i] == '-' && i > 0))
      return (false);
   }
   return (true);
}

bool isRealNumber(char* str) {
   int i, len = strlen(str);
   bool hasDecimal = false;
   if (len == 0)
   return (false);
   for (i = 0; i < len; i++) {
      if (str[i] != '0' && str[i] != '1' && str[i] != '2' && str[i] != '3' && str[i] != '4' && str[i]       != '5' && str[i] != '6' && str[i] != '7' && str[i] != '8'
      && str[i] != '9' && str[i] != '.' || (str[i] == '-' && i > 0))
      return (false);
         if (str[i] == '.')
      hasDecimal = true;
   }
   return (hasDecimal);
}

char* subString(char* str, int left, int right) {
   int i;
   char* subStr = (char*)malloc( sizeof(char) * (right - left + 2));
   for (i = left; i <= right; i++)
      subStr[i - left] = str[i];
   subStr[right - left + 1] = '\0';
   return (subStr);
}

void detectTokens(char* str) {
   int left = 0, right = 0;
   int length = strlen(str);
   while (right <= length && left <= right) {
      if (isValidDelimiter(str[right]) == false)
        right++;
      if (isValidDelimiter(str[right]) == true && left == right)
      {

         if (isOparith(str[right]) == true)
         {
             strcat(token,"oparith " );
         }

         else if (isSpecialChar(str[right]) == true)
         {
            char c = str[right];

            size_t len = strlen(token);

            /* one for extra char, one for trailing zero */

            token[len] = c;
            token[len+1] = ' ';

         }




         else if(str[right]=='>')
         {
             if (str[right+1]=='=')
             {
                strcat(token,"oprel " );
                right++;
             }

            else
            {
                size_t len = strlen(token);

            /* one for extra char, one for trailing zero */

                token[len] = '>';
                token[len+1] = ' ';
            }
         }


         else if(str[right]=='<')
         {
             if (str[right+1]=='=')
             {
                strcat(token,"oprel " );
                right++;
             }

            else
            {
                size_t len = strlen(token);

            /* one for extra char, one for trailing zero */

                token[len] = '<';
                token[len+1] = ' ';
            }
         }


         else if(str[right]==':')
         {
             if (str[right+1]=='=')
             {
                strcat(token,":= " );
                right++;
             }

            else
            {
                size_t len = strlen(token);

            /* one for extra char, one for trailing zero */

                token[len] = ':';
                token[len+1] = ' ';
            }


         }



         right++;
         left = right;

      } else if (isValidDelimiter(str[right]) == true && left != right || (right == length && left != right))
      {
         char* subStr = subString(str, left, right - 1);
         if (isValidKeyword(subStr) == true)
         {
             strcat(token,subStr);
             strcat(token," ");

         }

         else if (isValidInteger(subStr) == true)
         {
             strcat(token,"nb ");
         }

         else if (isRealNumber(subStr) == true)
             strcat(token,"nb " );
         else if (isvalidIdentifier(subStr) == true && isValidDelimiter(str[right - 1]) == false)
         {
             strcat(token,"id " );
         }

         else if (isvalidIdentifier(subStr) == false && isValidDelimiter(str[right - 1]) == false)
            printf("Invalid Identifier : '%s'\n", subStr);
         left = right;
      }
   }
   return;
}


