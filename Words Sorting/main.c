#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <locale.h>

#define MAXWORDS 500
#define MAXLEN 1024

char *wordptr[MAXWORDS];

int readwords(const char *fileName, char *wordptr[], int nlines);
char *wordprocess(char *word);
int comp(const char *word1, const char *word2);
void swap(char *v[], int i, int j);
void sort(char *v[], int left, int right);
void writelines(char *wordptr[], int nlines);

int main()
{
    setlocale(LC_ALL, "RUS");
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

void sort(char *v[], int left, int right)
{
    int i, last;

    if (left >= right)
        return;
    swap(v, left, (left + right) / 2);
    last = left;
    for (i = left + 1; i <= right; ++i)
    {
        if (comp(v[i], v[left]) < 0)
            swap(v, ++last, i);
    }
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