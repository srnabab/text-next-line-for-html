#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DEFAULT_CHARACTERS 50
#define DEFAULT_LANGUAGE "en"

int main(int argc, char * argv[])
{
    const char * supported_language[2] = {"en", "zh"};

    char * file_location;
    int character_per_line = DEFAULT_CHARACTERS;
    char language[2] = DEFAULT_LANGUAGE;

    if (argc < 2 || argc > 4)
    {
        puts("      html.exe [File Path]\nor\n      html.exe [File Path] [Characters per line](default for 50) [language](default for english)");
        puts("example:\n        html.exe file.txt 50 en");
        puts("      supported language: en, zh");
        return 0;
    }
    else if (argc == 2)
    {
        file_location = argv[1];
    }
    else if (argc == 3)
    {
        file_location = argv[1];
        character_per_line = atoi(argv[2]);
    }

    FILE * fp = NULL;
    if ((fp = fopen(file_location, "r+")) == NULL)
    {
        fprintf(stderr, "file: %s open failed\n", file_location);
    }

    size_t file_size = 0;
    fseek(fp, 0, SEEK_END);
    file_size = ftell(fp);
    rewind(fp);

    char txt[file_size];
    char txt_formated[file_size * 10];

    fread(txt, 1, file_size, fp);
    rewind(fp);

    int lines = file_size / character_per_line;
    unsigned int offset_formated = 0;
    unsigned int offset_txt = 0;
    for (int i = 0;i < lines;i++)
    {
        static unsigned int character_count = 0;
        unsigned int offset_formated1 = 0;
        int distance = 0;
        char * temp;
        if ((temp = (char *)memchr(txt + i * character_per_line, '\n', character_per_line)) != NULL)
        {
            distance = temp - (txt + i * character_per_line - offset_txt);
            offset_formated1 = character_per_line - distance;
            *temp = ' ';
        }
        snprintf(txt_formated + i * (character_per_line + 5) - offset_formated, character_per_line + 1 - offset_formated1, "%s", txt + i * character_per_line - offset_txt - offset_formated);
        offset_formated += offset_formated1;
        txt_formated[(i + 1) * (character_per_line + 5) - 5 - offset_formated] = '<';
        txt_formated[(i + 1) * (character_per_line + 5) - 4 - offset_formated] = 'b';
        txt_formated[(i + 1) * (character_per_line + 5) - 3 - offset_formated] = 'r';
        txt_formated[(i + 1) * (character_per_line + 5) - 2 - offset_formated] = '>';
        txt_formated[(i + 1) * (character_per_line + 5) - 1 - offset_formated] = '\n';
        if (txt[(i + 1) * character_per_line - offset_txt] == ' ' || txt[(i + 1) * character_per_line - offset_txt + 1] == ' ')
        {
            offset_txt++;
            txt_formated[(i + 1) * (character_per_line + 5) - 6] = '-';
        }
        if (i == lines - 1)
        {
            if (strlen(txt + (i + 1) * character_per_line - offset_txt - offset_formated) > character_per_line)
                lines++;
        }
    }
    sprintf(txt_formated + lines * (character_per_line + 5) - offset_formated, "%s", txt + lines * character_per_line - offset_txt - offset_formated);
    int length = strlen(txt_formated);
    txt_formated[length] = '<';
    txt_formated[length + 1] = 'b';
    txt_formated[length + 2] = 'r';
    txt_formated[length + 3] = '>';
    txt_formated[length + 4] = '\n';
    txt_formated[length + 5] = '\0';

    fwrite(txt_formated, 1, length + 5, fp);

    fclose(fp);

    return 0;
}
