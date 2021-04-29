#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>

// convert the hexadecimal string to int
int strToDec(char *str)
{
    int ans = 0;
    int j = 0;
    while (str[j] != '\0')
    {
        if (str[j] >= '0' && str[j] <= '9')
        {
            ans = ans * 16 + str[j] - 48;
        }
        else if (str[j] >= 'A' && str[j] <= 'Z')
        {
            ans = ans * 16 + str[j] - 55;
        }
        else if (str[j] >= 'a' && str[j] <= 'z')
        {
            ans = ans * 16 + str[j] - 87;
        }
        j++;
    }
    return ans;
}

// converting the decimal number to hexa decimal number
char *decToHex(int num)
{
    char *ans;
    ans = (char *)malloc(sizeof(char) * 10);
    int j = 0;
    int temp;
    while (num > 0)
    {
        temp = num % 16;
        if (temp < 10)
        {
            ans[j] = temp + 48;
        }
        else
        {
            ans[j] = 55 + temp;
        }
        j++;
        num /= 16;
    }
    char *ans2;
    ans2 = (char *)malloc(sizeof(char) * 10);
    int i = 0;
    j--;
    while (j >= 0)
    {
        ans2[i] = ans[j];
        i++;
        j--;
    }
    ans2[i] = '\0';
    return ans2;
}

// add 2 Hexadecimal numbers given in the form of strings
char *addHexNum(char *num1, char *num2)
{
    // converting the input hexa decimal numbers into decimal equivalents
    int num3, num4;
    num3 = strToDec(num1);
    num4 = strToDec(num2);

    // storing the addition result in num in decimal format
    int num = num3 + num4;

    // converting the decimal answer num to hexadecimal format
    char *ans;
    ans = decToHex(num);

    return ans;
}

// add a decimal number to a hexadecimal number
char *addHexToDec(char *num1, int num4)
{
    // converting the input hexa decimal numbers into decimal equivalents
    int num3 = strToDec(num1);

    // storing the addition result in num in decimal format
    int num = num3 + num4;

    // converting the decimal answer num to hexadecimal format
    char *ans;
    ans = decToHex(num);

    return ans;
}

// subtract 2 Hexadecimal numbers given in the form of strings
char *subHexNum(char *num1, char *num2)
{
    // converting the input hexa decimal numbers into decimal equivalents
    int num3, num4;
    num3 = strToDec(num1);
    num4 = strToDec(num2);

    // storing the addition result in num in decimal format
    int num = num3 - num4;

    // converting the decimal answer num to hexadecimal format
    char *ans;
    ans = decToHex(num);

    return ans;
}
