#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <locale.h>

#define MAXWORDS 1000
#define MAXLEN 256

int readlen();
int readwords(const char *fileName, char *wordptr[], int len, int maxwords);
char *wordprocess(char *word);
int comp(const char *word1, const char *word2);
void swap(char *v[], int i, int j);
void bubblesort(char **wordptr,
                int nwords,
                int (*comp)(const char *, const char *));
void writelines(char *wordptr[], int nlines);
void wordfree(char **wordptr, int nwords);

char *wordptr[MAXWORDS];


int main()
{
    setlocale(LC_ALL, "RUS");
    int nwords;
    int lenword;

    printf("Введите длину слов: ");
    lenword = readlen();
    if (lenword > 0)
        if ((nwords = readwords("file.txt", wordptr, lenword, MAXWORDS)) > 0)
        {
            printf("В файле найдено %d подходящих слов.\n", nwords);
            bubblesort(wordptr, nwords, comp);
            printf("\nОтсортированные слова:\n");
            writelines(wordptr, nwords);
            wordfree(wordptr, nwords);
        }
        else if (nwords == 0)
            printf("Ошибка: соответствующих слов не найдено.\n");
        else if (nwords == -1)
            printf("Ошибка: файл с названием \"file.txt\" не найден.\n");
        else if (nwords == -2)
            printf("Ошибка: в файле слишком много слов для сортировки.\n");
        else
            printf("Произошла неизвестная ошибка.\n");
    else
        printf("Ошибка: некорректное значение длины.\n");
    return 0;
}


int readlen()
{
    int n;

    scanf("%i", &n);
    if (!(n > 0))
        return -1;
    else if (n > MAXLEN)
        return -2;
    return n;
}

int readwords(const char *fileName, char *wordptr[], int len, int maxwords)
{
    FILE *ptrFile;
    int nwords;
    char *word, *token, line[MAXLEN];

    if ((ptrFile = fopen(fileName, "rt")) == NULL)
        return -1;

    nwords = 0;
    while (fgets(line, MAXLEN, ptrFile) != NULL)
    {
        token = strtok(line, " ");
        while (token != NULL)
        {
            if (nwords >= maxwords)
                return -2;
            else
            {
                word = wordprocess(token);
                if (strlen(word) != len)
                {
                    free(word);
                    word = NULL;
                }
                if (word != NULL)
                    wordptr[nwords++] = word;
            }
            token = strtok(NULL, " ");
        }
    }
    fclose(ptrFile);
    return nwords;
}

char *wordprocess(char *token)
{
    int i, j;
    int len;
    int inword;
    char *word;

    len = strlen(token);
    word = (char *)malloc(len + 1);
    if (word == NULL)
        return NULL;
    strcpy(word, token);

    inword = 0;
    for (i = 0; word[i] != '\0'; i++)
    {
        if (inword == 0 && isalpha(word[i]))
            inword = 1;
        else if (inword == 1 && !isalpha(word[i]) && (word[i] != '-'))
            inword = 0;
        if (!inword && (ispunct(word[i]) || iscntrl(word[i])))
        {
            for (j = i; j < len - 1; j++)
                word[j] = word[j + 1];
            len--;
            word[len] = '\0';
            i--;
        }
    }
    return word;
}

void writelines(char *wordptr[], int nlines)
{
    int i;

    for (i = 0; i < nlines; ++i)
        printf(" %s\n", wordptr[i]);
}

int comp(const char *word1, const char *word2)
{
    int ch1, ch2;
    int i;

    for (i = 0; word1[i] != '\0' || word2[i] != '\0'; i++)
    {
        if (isupper(word1[i]))
            ch1 = word1[i] - 'A';
        else if (islower(word1[i]))
            ch1 = word1[i] - 'a';
        else
            ch1 = word1[i];

        if (isupper(word2[i]))
            ch2 = word2[i] - 'A';
        else if (islower(word2[i]))
            ch2 = word2[i] - 'a';
        else
            ch2 = word2[i];

        if (ch1 > ch2)
            return 1;
        else if (ch1 < ch2)
            return -1;
    }
    return 0;
}

void swap(char *v[], int i, int j)
{
    char *tmp;

    tmp = v[i];
    v[i] = v[j];
    v[j] = tmp;
}

void wordfree(char **wordptr, int nwords)
{
    int i;

    for (i = 0; i < nwords; i++)
        free(wordptr[i]);
}

void bubblesort(char **wordptr,
                int nwords,
                int (*comp)(const char *, const char *))
{
    int i, j;
    int flag;

    for (i = 0; i < nwords; i++)
    {
        flag = 1;
        for (j = 0; j < nwords - (i + 1); j++)
            if (comp(wordptr[j], wordptr[j + 1]) > 0)
            {
                flag = 0;
                swap(wordptr, j, j + 1);
            }
        if (flag)
            break;
    }
}