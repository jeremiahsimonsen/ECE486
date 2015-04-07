
/*!
 * @file
 * 
 * @brief Routines to convert float and int types to ascii strings
 * 
 * @author Don Hummels (Modified from online source)
 * 
 * @date March 29 2015
 * 
 * These routines convert numeric values (type float or int) into 
 * null-terminated character strings.  Callers must provide storage 
 * for the result.  
 * 
 * The routines are developed to address failures of the 
 * newlib printf() and sprintf() functions to correctly support the %d and 
 * %f format specifiers.
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

#ifndef ECE486_NUMTOSTR_H
#define ECE486_NUMTOSTR_H

/*!
 * @brief Convert an "int" to a null-terminated ascii string.
 * 
 * Converts a given integer x to string str[]. 
 * 
 * Example:
 * @verbatim
 *   char str1[20];
 *   int x=23;
 *   ...
 *   inttostr(x, str1);
 * @endverbatim
 * will result in str1="23" 
 * 
 * @returns The pointer to the string result is returned.
 * 
 * @note The user must supply the memory for the resulting string.  No action
 * is taken if the input pointer is NULL.  
 */
char *inttostr(
  int x, 	//!< [in] Integer value to be converted
  char *str);   //!< [out] Pointer to output string location
 
/*!
 * @brief Convert an "float" to a null-terminated ascii string.
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
 * @verbatim
 *   char str1[23], str2[23];
 *   float x=3.98765;
 *   ...
 *   floattostr(x, str1, 2);
 *   floattostr(-x, str2, 4);
 * @endverbatim
 * will result in str1="3.98" and str2="-3.9876"
 * 
 * @returns The pointer to the string result is returned.
 * 
 * @note The user must supply the memory for the resulting string.  No action
 * is taken if the input pointer is NULL.  Only up to 10 digits following the 
 * decimal is supported.  Input values larger than 2147480000.0f result in the string 
 * "OVERFLOW"
 */
char *floattostr(
  float x, 		//!< [in] float value to be converted
  char  *str,    	//!< [out] Pointer to output string location
  int   afterpoint);	//!< [in] Minimum number of digits

#endif