#include <stdio.h>
#include "stack.h"


/**
 * @question
 * input an expression, output an answer
 * @example1
 * input:  "1+2*3-4"
 * output: 3
 * @example2
 * input:  "34+13*9+44-12/3"
 * output: 191
 *
 * @method1
 * tokenizer --> num op num op ... --> parser --> bitree --> interpreter (depth-first traversal) --> output
 *
 * @method2
 * double stack, stack1 for num, stack2 for op
 * 1. tokenizer (one token one time, either num or op)
 * 2. if num, push into stack1
 *    if op
 *       if prio(op)<=prio(stack2_top), pop two num and one op, calculate, push the result into stack1
 *       else, push into stack2
 * 3. the final result is into stack1
 *
 * @note
 * Method1 is what compiler do, it's a big&full solution. If we just need to parse an simple expression(num can only be positive integer), method2 is enough.
 * In using method2, stack maybe overflow.
 */

static int isDigit(char c)
{
    if ('0' <= c && c <= '9')
        return 1;
    return 0;
}

static int isOp(char c)
{
    if (c == '+' || c == '-' || c == '*' || c == '/')
        return 1;
    return 0;
}

static int getOpPrio(char c)
{
    int res = 0;

    switch (c) {
    case '+':
        res = 1;
        break;
    case '-':
        res = 1;
        break;
    case '*':
        res = 2;
        break;
    case '/':
        res = 2;
        break;
    default:
        break;
    }

    return res;
}

static int calculate(int num1, char op, int num2)
{
    int res = 0;

    if (op == '+')
        res = num1 + num2;
    else if (op == '-')
        res = num1 - num2;
    else if (op == '*')
        res = num1 * num2;
    else if (op == '/')
        res = num1 / num2;

    return res;
}


int parseExpr(char* str)
{
    Stack_t stack1, stack2;
    char c;
    int num, num2;
    char op;
    int res;

    StackInit(&stack1);
    StackInit(&stack2);

    num = 0;
    while ((c = *str) != '\0') {
        if (isDigit(c)) {
            num = num * 10 + c - '0';
            str++;
        }
        else if (isOp(c)) {
            res = StackGetTop(&stack2, (void**)&op);
            if (res == 0 || getOpPrio(c) > getOpPrio(op)) {
                StackPush(&stack1, (void*)num);
                StackPush(&stack2, (void*)c);
                num = 0;
                str++;
            }
            else {
                StackPop(&stack2, (void**)&op);
                StackPop(&stack1, (void**)&num2);
                num = calculate(num2, op, num);
            }
        }
        else {
            printf("error output\r\n");
            return 0;
        }
    }

    while (!StackIsEmpty(&stack1)) {
        StackPop(&stack2, (void**)&op);
        StackPop(&stack1, (void**)&num2);
        num = calculate(num2, op, num);
    }
    
    return num;
}


int main()
{
    int ret;

    ret = parseExpr("1+2*3-4");
    printf("res: %d\r\n", ret);
    ret = parseExpr("34+13*9+44-12/3");
    printf("res: %d\r\n", ret);
}