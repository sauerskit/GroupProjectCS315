int main() 
{
    FILE *in;
    FILE *out;
    char inName[100];
    char outName[100];
    char current;
    char previous;
    int count;

    // ask user for file names
    printf("Enter input file name: ");
    scanf("%s", inName);
    printf("Enter output file name: ");
    scanf("%s", outName);

    // open input file to read, output file to write
    in = fopen(inName, "r");
    out = fopen(outName, "w");

    if (in == NULL || out == NULL) 
    {
        printf("Error opening file!\n");
        return 1;
    }

    // read first character
    previous = fgetc(in);
    if (previous == EOF) 
    {
        printf("File is empty!\n");
        return 1;
    }

    count = 1;

    // read the rest of the file
    while ((current = fgetc(in)) != EOF) 
    {
        if (current == previous) 
        { // if same as previous character, increase count
            count++;
        } 
        else 
        { // if different, write count and character to output file
            fprintf(out, "%d%c", count, previous);
            previous = current;
            count = 1;
        }
    }

    
    fprintf(out, "%d%c", count, previous); // write the last group of characters

    printf("File compressed successfully!\n");

    fclose(in); //close file
    fclose(out); //close file
    
    return 0;
}
