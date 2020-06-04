
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
typedef struct Decimal
{
    int intPart;
    int fractionPart; 
    int pointCount;
    char sign;
};



struct Decimal summarize(struct Decimal left,struct Decimal right);
struct Decimal subtract(struct Decimal left, struct Decimal right);
struct Decimal multiply(struct Decimal left, struct Decimal right);
struct Decimal clearing(struct Decimal dec);//приводим число в удобоваримый вид
struct Decimal readDecimal(char* numb);
void printDecimal(struct Decimal dec);

//вспомогательные функции
struct Decimal sum(struct Decimal left, struct Decimal right);
struct Decimal sub(struct Decimal left, struct Decimal right);
struct Decimal cut(struct Decimal number);
struct Decimal join(struct Decimal number);
int  numberOfDigit(int digit);
int power(int num, int p);
int compare(struct Decimal left, struct Decimal right);



struct Decimal readDecimal(char* numb)
{
     struct Decimal dec;
    int size = strlen(numb);
    char intPart[9];
    char fractionPart[5];
    int flag = 0;
    int start = 0;

    if (numb[0] =='-')
    {
        start = 1;
        dec.sign = '-';
    }
    else
    {
        dec.sign = '+';
        
    }


    for (int i = start; i < size; i++) // поиск целой части
    {
        if (numb[i] == '.')//128.89 flag = 3
        {
            flag = i;
            break;
        }
        if (dec.sign == '-')
        {
         intPart[i-start] = numb[i];
        }
        else
        {
            intPart[i] = numb[i];
        }
       
      
    }
    dec.pointCount = size - flag - 1;
    
    for (int i =flag+1; i < size; i++)
    {
        fractionPart[i -flag-1] = numb[i];
    }
    dec.intPart = atoi(intPart);
    dec.fractionPart = atoi(fractionPart);
    return dec;

}




void printDecimal(struct Decimal dec)
    {
    if (dec.sign == '-')
    {
        printf("-");
    }
     printf("%i,",dec.intPart);

     int buf = numberOfDigit(dec.fractionPart);
     buf = dec.pointCount - buf;
     if (dec.fractionPart !=0)
     {  while (buf != 0)
       {
         buf--;
         printf("0");
       }
     }
     

     printf("%i", dec.fractionPart);
}




struct Decimal summarize(struct Decimal left, struct Decimal right)
{
    struct Decimal result;
    char signs[2] = { left.sign,right.sign };
    int variable = 0;
    if (signs[0]=='+' && signs[1] == '+')
    {
        variable = 1;
    }
    else if (signs[0] == '+' && signs[1] == '-')
    {
        variable = 2;
    }
    else if (signs[0] == '-' && signs[1] == '+')
    {
        variable = 3;
    }
    else
    {
        variable = 4;
    }

    switch(variable)
    {
    case 1:
      result = sum(left, right);
    break;
    case 2:
        right.sign = '+';
        result = subtract(left, right);
        break;
    case 3:
        left.sign = '+';
        result = subtract(right, left);
        break;
    case 4:
        left.sign = '+';
        right.sign = '+';
        result = sum(left, right);
        result.sign = '-';
        break;
    }
    return result;
}




struct Decimal sum(struct Decimal left, struct Decimal right)
{
    struct Decimal sum;
    sum.intPart = left.intPart + right.intPart;
    sum.fractionPart = left.fractionPart*power(10, right.pointCount) +
        right.fractionPart*power(10, left.pointCount);
    sum.pointCount = left.pointCount + right.pointCount;
    if (sum.fractionPart == power(10, sum.pointCount))
    {
        sum.intPart++;
        sum.fractionPart = 0;
        sum.pointCount = 1;
    }
    if (sum.fractionPart > power(10, sum.pointCount))
    {
        sum.intPart++;
        sum.fractionPart = sum.fractionPart - power(10, sum.pointCount);
        sum.pointCount;
    }

    sum = clearing(sum);

    return sum;
}


struct Decimal subtract(struct Decimal left, struct Decimal right)
{
    struct Decimal result;
    int comp = compare(left, right);
    if (comp == 0)
    {
        result.intPart = 0;
        result.fractionPart = 0;
        result.pointCount = 1;
        result.sign = '+';
        return result;
    }
    if (comp == 2)
    {
        struct Decimal buf = left;
        left = right;
        right = buf;
        
        result = subtract(left, right);
        result.sign = '-';
        return result;
    }

    char signs[2] = { left.sign,right.sign };
    int variable = 0;
    if (signs[0] == '+' && signs[1] == '+')
    {
        variable = 1;
    }
    else if (signs[0] == '+' && signs[1] == '-')
    {
        variable = 2;
    }
    else if (signs[0] == '-' && signs[1] == '-')
    {
        variable = 3;
    }
     
    switch (variable)
    {
    case 1:
        result = sub(left, right);
        break;
    case 2:
        right.sign = '+';
        result = sum(left, right);
        break;
    case 3:
        right.sign = '+';
        left.sign = '+';
        result = sub(right, left);
        break;
    }
    return result;

}



struct Decimal sub(struct Decimal left, struct Decimal right)
{
    struct Decimal result;
    result.intPart = left.intPart - right.intPart;
    result.fractionPart = left.fractionPart*power(10, right.pointCount) -
        right.fractionPart*power(10, left.pointCount);
    result.pointCount = left.pointCount + right.pointCount;
    if (result.fractionPart < 0)
    {
        result.intPart--;
        result.fractionPart = power(10, result.pointCount) + result.fractionPart;
    }
    if (result.intPart < 0)
    {
        result.sign = '-';
        result.intPart = abs(result.intPart);
    }
    else
    {
        result.sign = '+';
    }
    result = clearing(result);
    return result;
}




struct Decimal multiply(struct Decimal left, struct Decimal right)
{
    struct Decimal result;
    if (left.sign == '+' && right.sign =='+' || 
        left.sign == '-' && right.sign == '-')
    {
        result.sign = '+';
    }
    else
    {
        result.sign = '-';
    }
    left = cut(left);
    right = cut(right);
    result.intPart = left.intPart * right.intPart;
    result.pointCount = left.pointCount + right.pointCount;
    result = join(result);
    clearing(result);
    return result;
}




int power(int num, int p)
{
    int pow = 1;
    for (int i = 0; i < p; i++)
    {
        pow = pow * num;
    }
    return pow;
}




struct Decimal clearing(struct Decimal dec)
{
    int buf = dec.pointCount;
    for (int i = 0; i < dec.pointCount; i++)
    {
        if (dec.fractionPart % 10 == 0)
        {
            dec.fractionPart = dec.fractionPart / 10;
            buf--;
        }
    }
    dec.pointCount = buf;
    return dec;
}



int  numberOfDigit(int digit)
{
    int count = 0;
    while (digit !=0 )
    {
        digit /= 10;
        count++;
    }
}


int compare(struct Decimal left, struct Decimal right)
{
    int bufInt = 0;
    int bufFraction = 0;
    //сравниваем знаки
    if (left.sign == '+' && right.sign =='-')
    {
        return 1;
    }
    else if (left.sign == '-' && right.sign == '+')
    {
        return 2;
    }// если знаки равны ,сравниваем числа
    else if (left.sign == '+' && right.sign == '+')
    {
        bufInt = (left.intPart > right.intPart) ? 1 : 2;
        if (bufInt ==2)
        bufInt = (left.intPart == right.intPart ) ? 0 : 2;
        if (bufInt == 0)
        {
            bufFraction = (left.fractionPart > right.fractionPart) ? 1 : 2;
            if(bufFraction == 2)
            bufFraction = (left.fractionPart == right.fractionPart) ? 0 : 2;
            return bufFraction;
        }
        else return bufInt;
    }
    else
    {
        bufInt = (left.intPart < right.intPart) ? 1 : 2;
        if (bufInt == 2)
        bufInt = (left.intPart == right.intPart) ? 0 : 2;
        if (bufInt == 0)
        {
            bufFraction = (left.fractionPart < right.fractionPart) ? 1 : 2;
            if (bufFraction == 2)
            bufFraction = (left.fractionPart = right.fractionPart) ? 0 : 2;
            return bufFraction;
        }
        else return bufInt;
    }
}



struct Decimal cut(struct Decimal number)
{
    number.intPart = number.intPart * power(10, number.pointCount) +
        number.fractionPart;
    number.fractionPart = 0;
    return number;

}




struct Decimal join(struct Decimal number)
{
    number.fractionPart = number.intPart % power(10, number.pointCount);
    number.intPart = number.intPart / power(10, number.pointCount);
    return number;
}

int main()
{
    char number1[14];
    char number2[14];
    printf("write two numbers :");
    scanf("%s",&number1);
    scanf("%s", &number2);
    printf("\n");
    struct Decimal dec = readDecimal(number1);
    struct Decimal dec1 = readDecimal(number2);
   struct Decimal a = summarize(dec, dec1);
   struct Decimal b = subtract(dec, dec1);
   struct Decimal c = multiply(dec, dec1);

   printf("summarize : "); printDecimal(a); printf("\n");
   printf("subtract : "); printDecimal(b); printf("\n");
   printf("multiply : "); printDecimal(c); printf("\n");
    return 0;
}
