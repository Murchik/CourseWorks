#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>

#define MAXWORDS 5000

char *wordptr[MAXWORDS];

int readwords(const char *fileName, char *wordptr[], int nlines);
void writelines(char *wordptr[], int nlines);
void _qsort(char *v[], int left, int right);
void swap(char *v[], int i, int j);

int main()
{
    int nwords;

    if ((nwords = readwords("file.txt", wordptr, MAXWORDS)) >= 0)
    {
        _qsort(wordptr, 0, nwords - 1);
        writelines(wordptr, nwords);
        return 0;
    }
    else
    {
        printf("error: input too big to sort\n");
        return 1;
    }

    return 0;
}

#define MAXLEN 1024

int readwords(const char *fileName, char *wordptr[], int maxwords)
{
    FILE *ptrFile;
    int len, nlines, nwords;
    char *p, *lineptr, line[MAXLEN];

    if ((ptrFile = fopen(fileName, "rt")) == NULL)
        return -1;

    nwords = 0;
    nlines = 0;
    while (fgets(line, MAXLEN, ptrFile) != NULL)
    {
        lineptr = strtok(line, " ");
        while (lineptr != NULL)
        {
            if (nwords >= maxwords || (p = (char *)malloc(len)) == NULL)
                return -1;
            else
            {
                strcpy(p, lineptr);
                wordptr[nwords++] = p;
            }
            lineptr = strtok(NULL, " ");
        }
        ++nlines;
    }

    fclose(ptrFile);
    return nwords;
}

void writelines(char *wordptr[], int nlines)
{
    int i;

    for (i = 0; i < nlines; ++i)
        printf("%s\n", wordptr[i]);
}

void _qsort(char *v[], int left, int right)
{
    int i, last;
    if (left >= right)
        return;
    swap(v, left, (left + right) / 2);
    last = left;
    for (i = left + 1; i <= right; ++i)
    {
        if (strcmp(v[i], v[left]) < 0)
            swap(v, ++last, i);
    }
    swap(v, left, last);
    _qsort(v, left, last - 1);
    _qsort(v, last + 1, right);
}

void swap(char *v[], int i, int j)
{
    char *tmp;

    tmp = v[i];
    v[i] = v[j];
    v[j] = tmp;
}