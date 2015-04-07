/*!
 * @file
 * 
 * @brief Implementation and support functions for floattostr() and inttostr()
 * 
 * @author Don Hummels (Modified from online source)
 * 
 * @date March 29 2015
 * 
 * These routines convert numeric values (type float or int) into 
 * null-terminated character strings.  Callers must provide storage 
 * for the result.  
 * 
 * The implementations here were essentially stolen from an online source, and
 * modified by Hummels to: a) support negative values, b) return the 
 * pointer to the result, and c) avoid use of the pow() function.  
 * 
 * The orginal code is from 
 * http://www.geeksforgeeks.org/convert-floating-point-number-string/
 * 
 * Original Article and code contributed by Jayasantosh Samal.
 * 
 */

#include "numtostr.h"
#include <string.h>

static char ovfl_error[] = "OVERFLOW";

// reverses a string 'str' of length 'len'
static void reverse(char *str, int len)
{
    int i=0, j=len-1, temp;
    while (i<j)
    {
        temp = str[i];
        str[i] = str[j];
        str[j] = temp;
        i++; j--;
    }
}

/* Local version of inttostr() that works for positive integers only, and returns the
 * number of characters used in the string representation.
 */
static int intToStr(int x, char str[], int d)
{
    int i = 0;
    while (x) {
        str[i++] = (x%10) + '0';
        x = x/10;
    }
 
    // If number of digits required is more, then
    // add 0s at the beginning
    while (i < d)
        str[i++] = '0';
 
    reverse(str, i);
    str[i] = '\0';
    return i;
}

/*
 * Convert an "int" to a null-terminated ascii string.
 * 
 * 
 * Example:
 *   char str1[20];
 *   int x=23;
 *   ...
 *   inttostr(x, str1);
 * will result in str1="23"
 * 
 * The pointer to the string result is returned.
 * 
 * The user must supply the memory for the resulting string.  No action
 * is taken if the input pointer is NULL. 
 */
char *inttostr(
  int x, 	// Integer value to be converted
  char *str)    // Pointer to output string location
{    
    if (str == NULL) return(NULL);
    if (x<0) {
      str[0] = '-';
      intToStr(-x,str+1,1);
      return str;
    }
    intToStr(x,str,1);
    return str;
} 




/*
 * Convert an "float" to a null-terminated ascii string.
 * 
 * Converts a given float x to string null-terminated string
 * with a requested number of digits
 * following the decimal point.  This routine is limited to 10 digits before and
 * after the decimal point, so that the final result can always be stored in a 
 * character array with 23 elements (sign, 10 digits before the decimal, '.', 
 * up to 10 digits after the decimal, and '\0' termination).  Values larger than
 * 2147480000.0f are converted to "OVERFLOW".  No rounding is performed.
 * 
 * Example:
 *   char str1[23], str2[23];
 *   float x=3.98765;
 *   ...
 *   floattostr(x, str1, 2);
 *   floattostr(-x, str2, 4);
 * will result in str1="3.98" and str2="-3.9876"
 * 
 * The pointer to the string result is returned.
 * 
 * The user must supply the memory for the resulting string.  No action
 * is taken if the input pointer is NULL.  Only up to 10 digits following the 
 * decimal is supported.  Input values larger than 2147480000.0f result in the string 
 * "OVERFLOW"
 */
char *floattostr(
  float x, 		// float value to be converted
  char *str,    	// Pointer to output string location
  int afterpoint)	// Minimum number of digits
{
  char *res = str; 
  int j;
  
  if (str == NULL) return(NULL);	// Don't scribble 
  
  if (afterpoint>10) afterpoint=10;
  
  if (x<0) {
    str[0] = '-';
    str++;
    x = -x;
  }
  
  if (x>2147480000.0f) {	// Must be able to store integer part in an int
    return strcpy( res, ovfl_error );
  }
  
  // Extract integer part
  int ipart = (int)x;
 
  // Extract floating part
  float fpart = x - (float)ipart;
 
  // convert integer part to string
  int i = intToStr(ipart, str, 1);
 
  // check for display option after point
  if (afterpoint != 0) {
    str[i] = '.';  // add dot
 
    // Get the value of fraction part up to given no.
    // of points after dot. The third parameter is needed
    // to handle cases like 233.007
    for (j=0; j<afterpoint; j++) fpart *= 10;
 
    intToStr((int)fpart, str + i + 1, afterpoint);
    }
  
  return res;
}









