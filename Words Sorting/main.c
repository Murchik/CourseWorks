#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <locale.h>

#define MAXWORDS 5000
#define MAXLEN 1024

struct word
{
    char *ch; // Указатель на слово
    int num;  // Сколько раз встречается в тексте
};

char *wordptr[MAXWORDS];

int readwords(const char *fileName, char *wordptr[], int nlines);
void writelines(char *wordptr[], int nlines);
void sort(char *v[], int left, int right);
void swap(char *v[], int i, int j);
char *wordprocess(char *word);

int main()
{
    int nwords;

    if ((nwords = readwords("file.txt", wordptr, MAXWORDS)) >= 0)
    {
        sort(wordptr, 0, nwords - 1);
        writelines(wordptr, nwords);
    }
    else
        printf("error: input too big to sort\n");

    return 0;
}

int readwords(const char *fileName, char *wordptr[], int maxwords)
{
    FILE *ptrFile;
    int len, nwords;
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
    word = (char *)malloc(len);
    if (word == NULL)
        return NULL;
    strcpy(word, token);

    inword = 0;
    for (i = 0; word[i] != '\0'; i++)
    {
        if (inword == 0 && isalpha(word[i]))
            inword = 1;
        else if (inword == 1 && !isalpha(word[i]) && !isalpha(word[i + 1]))
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
    if (strlen(word) == 0)
        return NULL;
    return word;
}

void writelines(char *wordptr[], int nlines)
{
    int i;

    for (i = 0; i < nlines; ++i)
        printf("%s\n", wordptr[i]);
}

void sort(char *v[], int left, int right)
{
    int i, last;

    if (left >= right)
        return;
    swap(v, left, (left + right) / 2);
    last = left;
    for (i = left + 1; i <= right; ++i)
        if (strcmp(v[i], v[left]) < 0)
            swap(v, ++last, i);
    swap(v, left, last);
    sort(v, left, last - 1);
    sort(v, last + 1, right);
}

void swap(char *v[], int i, int j)
{
    char *tmp;

    tmp = v[i];
    v[i] = v[j];
    v[j] = tmp;
}