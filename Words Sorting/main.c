#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <locale.h>

char **readwords(const char *fileName, int *nwords, int len);
char *wordprocess(char *word);
void bubblesort(char **wordptr,
                int nwords,
                int (*comp)(const char *, const char *));
int comp(const char *word1, const char *word2);
void writelines(char **wordptr, int nlines);
void wordfree(char **wordptr, int nwords);

int main()
{
    setlocale(LC_ALL, "RUS");
    char **wordptr = NULL;
    int nwords;
    int lenword;

    printf("Введите длину слов: ");
    scanf("%i", &lenword);

    if (lenword > 0)
    {
        if ((wordptr = readwords("file.txt", &nwords, lenword)) != NULL)
        {
            bubblesort(wordptr, nwords, comp);
            printf("Найдено подходящих слов в файле: %d.\n", nwords);
            printf("\nОтсортированные слова:\n");
            writelines(wordptr, nwords);
            wordfree(wordptr, nwords);
        }
        else if (nwords == 0)
            printf("Cоответствующих слов не найдено.\n");
        else if (nwords == -1)
            printf("Ошибка: не удалось открыть входной файл \"file.txt\".\n");
        else if (nwords == -2)
            printf("Ошибка: количество считанных слов слишком велико.\n");
        else if (nwords == -3)
            printf("Произошла ошибка при чтении файла.\n");
        else
            printf("Произошла неизвестная ошибка.\n");
    }
    else
        printf("Ошибка: некорректное значение длины.\n");
    return 0;
}

char **readwords(const char *fileName, int *nwords, int len)
{
    FILE *ptrFile;
    char **wordptr;
    int numw;
    char *word, *token, line[256];

    if ((ptrFile = fopen(fileName, "rt")) == NULL)
    {
        *nwords = -1;
        return NULL;
    }

    wordptr = NULL;
    numw = 0;
    while (fgets(line, 256, ptrFile) != NULL)
    {
        token = strtok(line, " ");
        while (token != NULL)
        {
            word = wordprocess(token);
            if (word != NULL)
                if (strlen(word) == len)
                {
                    numw++;
                    wordptr = (char **)realloc(wordptr, sizeof(char *) * numw);
                    if (wordptr != NULL)
                        wordptr[numw - 1] = word;
                    else
                    {
                        wordfree(wordptr, numw);
                        *nwords = -2;
                        return NULL;
                    }
                }
                else
                {
                    free(word);
                    word = NULL;
                }
            token = strtok(NULL, " ");
        }
    }
    if (!feof(ptrFile))
    {
        wordfree(wordptr, numw);
        *nwords = -3;
        return NULL;
    }
    fclose(ptrFile);
    *nwords = numw;
    return wordptr;
}

char *wordprocess(char *token)
{
    char *word;
    int ch;
    int len;
    int i;

    len = strlen(token);
    ch = (unsigned char)token[0];
    while (!isalpha(ch) && len > 0)
    {
        token++;
        len--;
        ch = (unsigned char)token[0];
    }

    ch = (unsigned char)token[len - 1];
    while (!isalpha(ch) && len > 0)
    {
        len--;
        ch = (unsigned char)token[len - 1];
    }

    word = (char *)malloc(len + 1);
    if (word != NULL)
    {
        for (i = 0; i < len; ++i)
            word[i] = token[i];
        word[len] = '\0';
    }

    return word;
}

int comp(const char *word1, const char *word2)
{
    int ch1, ch2;
    int i;

    for (i = 0; word1[i] != '\0' && word2[i] != '\0'; ++i)
    {
        ch1 = (unsigned char)word1[i];
        ch2 = (unsigned char)word2[i];
        ch1 = tolower(ch1);
        ch2 = tolower(ch2);
        if (ch1 > ch2)
            return 1;
        else if (ch1 < ch2)
            return -1;
    }

    if (word1[i] == '\0' && word2[i] == '\0')
        return 0;
    else if (word1[i] == '\0')
        return -1;
    else
        return 1;
}

void bubblesort(char **wordptr,
                int nwords,
                int (*comp)(const char *, const char *))
{
    int i, j;
    int flag;
    char *tmp;

    for (i = 0; i < nwords; i++)
    {
        flag = 1;
        for (j = 0; j < nwords - (i + 1); j++)
            if (comp(wordptr[j], wordptr[j + 1]) > 0)
            {
                flag = 0;
                tmp = wordptr[j];
                wordptr[j] = wordptr[j + 1];
                wordptr[j + 1] = tmp;
            }
        if (flag)
            break;
    }
}

void writelines(char **wordptr, int nlines)
{
    int i;

    for (i = 0; i < nlines; ++i)
        printf(" %s\n", wordptr[i]);
}

void wordfree(char **wordptr, int nwords)
{
    int i;

    for (i = 0; i < nwords; i++)
        free(wordptr[i]);
}