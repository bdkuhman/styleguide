#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <regex.h>

#define KNRM  "\x1B[0m"
#define KRED  "\x1B[31m"
#define KGRN  "\x1B[32m"
#define KGRN_W  "\x1B[32;3m"
#define KYEL  "\x1B[33m"
#define KBLU  "\x1B[34m"
#define KMAG  "\x1B[35m"
#define KCYN_W  "\x1B[36;3m"
#define KWHT  "\x1B[37m"

enum {SUPPRESS, VERBOSE};

char* suppress = "-s";
char* verbose = "-v";

int mode;
char* filestring;

//regex to prevent matching string literals: might not work right.
 //   /(?=([^"\\]*(\\.|"([^"\\]*\\.)*[^"\\]*"))*[^"]*$)/
char* sliteral = "(?=([^\"\\\\]*(\\\\.|\"([^\"\\\\]*\\\\.)*[^\"\\\\]*\"))*[^\"]*$)";

char* keywords[] = {"#include", //0		/(#\w)/
					"){", 		//1		/((\)|\b)[^\S\n]*{)/
					"int ", 	//2     /((\s|\()(int|float|double|char|short|long)\s)/
					"include<", //3		((include|define)[<"])
					"long ", 	//4		see 2
					"short ", 	//5		see 2
					"_",  		//6
					") {", 		//7		see 1
					"enum{", 	//8		see 1
					"enum {", 	//9		see 1
					"struct {", //10    see 1
					";//",		//11	/(\s?(\/(\/|\*))(\S)?)/ 		also catches /*
					"{\"",		//12	/("[^\S\n]*}|[^\S\n]*{")/
					"\"}",		//13	see 12
					"\n {",		//14	see 12
					"}//",		//15	see 11
					"{//",		//16	see 11
					"#define",	//17	see 0, 3
					"()",		//18	/^.*\b\s*\((\s*)\)/   --matches empty parameter function declarations
                    "//",        //19	see 11
                    ","         //20	/(,\S)/

								//21	(?<!(typedef[\s]+))(enum|struct) //should check for typedef but not all vesions support it.
								//22
					};

const int keywordsSize = 21;
char *trim(char *str){

    size_t len = 0;
    char *frontp = str;
    char *endp = NULL;

    if( str == NULL ) { return NULL; }
    if( str[0] == '\0' ) { return str; }

    len = strlen(str);
    endp = str + len;

    while( isspace((unsigned char) *frontp) ) { ++frontp; }
    if( endp != frontp )
    {
        while( isspace((unsigned char) *(--endp)) && endp != frontp ) {}
    }

    if( str + len - 1 != endp )
            *(endp + 1) = '\0';
    else if( frontp != str &&  endp == frontp )
            *str = '\0';

    endp = str;
    if( frontp != str )
    {
            while( *frontp ) { *endp++ = *frontp++; }
            *endp = '\0';
    }


    return str;
}

char *errormsg(int keyword){

	char* toReturn;

	switch(keyword){

		case 0:
			toReturn = "Add a space between '#' and include";
			break;
		case 1:
			toReturn = "Add a newline between ')' and '{'";
			break;
		case 2:
			toReturn = KCYN_W "Warning" KNRM ": Is 'int' the most specific datatype for this instance?";
			break;
		case 3:
			toReturn = "Add a space between 'include' and '<'";
			break;
		case 4:
			toReturn = KCYN_W "Warning:" KNRM " Is 'long' the most specific datatype for this instance?";
			break;
		case 5:
			toReturn = KCYN_W "Warning:" KNRM " Is 'short' the most specific datatype for this instance?";
			break;
		case 6:
			toReturn = KCYN_W "Warning:" KNRM " Are you using camelCase?";
			break;
		case 7:
			toReturn = "Add a newline between ')' and '{'";
			break;
		case 8:
			toReturn = "typedef your enum and add a space ";
			break;
		case 9:
			toReturn = "typedef your enum";
			break;
		case 10:
			toReturn = "Don't use anonymous structures";
			break;
		case 11:
			toReturn = "Put a tab (or space) before inline comments";
			break;
		case 12:
			toReturn = "Add a newline afer '}'";
			break;
		case 13:
			toReturn = "Add a newline before '}'";
			break;
		case 14:
			toReturn = KCYN_W "Warning" KNRM ": conditionals should be nested and brackets/text should be aligned (do you have extra whitespace?)";
			break;
		case 15:
			toReturn = "Put a tab (or space) before inline comments";
			break;
		case 16:
			toReturn = "Put a tab (or space) before inline comments";
			break;
		case 17:
			toReturn = "Add a space between '#' and define";
			break;
		case 18:
			toReturn = KCYN_W "Warning" KNRM ": If your function does not take any parameters, be sure to use void in the parenthesis.";
			break;
        case 19:
			toReturn = "Add space between slashes and comment.";
            break;
        case 20:
            toReturn = "Add a space after ','";
            break;
		default:
			toReturn = "Undefined error";
			break;
	}

	return toReturn;
}

int search(char *fname) {
	FILE *fp;
	int line_num = 1;
	char temp[512];

	if((fp = fopen(fname, "r")) == NULL) {
		perror(KRED "Error" KNRM ": Could not read from target file");
		exit(1);
	}

	while(fgets(temp, 512, fp) != NULL)
	{
		for(int i = 0; i < keywordsSize; i++)
		{
            char* index = strstr(temp,keywords[i]);
			if(index != NULL)
			{
				if(temp[0] != '/' && temp[1] != '/' && i != 19)	// if not a comment
				{
                    if (i != 20)
                    {
    					if(mode == VERBOSE)
    					{
    						printf(KRED "Error" KNRM " on line %d:\n", line_num);
    						trim(temp);
    						printf(KGRN "    %s\n" KNRM, temp);
    						printf("    %s\n\n", errormsg(i));
    					}
    					else
    					{
    						if(i != 2 && i != 4 && i != 5 && i != 6 && i != 14 && i != 18)
    						{
    							printf(KRED "Error" KNRM " on line %d:\n", line_num);
    							trim(temp);
    							printf(KGRN "    %s\n" KNRM, temp);
    							printf("    %s\n\n", errormsg(i));
    						}
    					}
                    }
                    if (i == 20 && temp[index-temp + 1] != ' ')
                    {
                            printf(KRED "Error" KNRM " on line %d:\n", line_num);
    						trim(temp);
    						printf(KGRN "    %s\n" KNRM, temp);
	    					printf("    %s\n\n", errormsg(i));

                    }

				}
                else
                {
                        if(temp[index - temp + 2]  != ' ')
                        {
                            printf(KRED "Error" KNRM " on line %d:\n", line_num);
    						trim(temp);
    						printf(KGRN "    %s\n" KNRM, temp);
	    					printf("    %s\n\n", errormsg(i));
                        }

			    }

            }
		}
		line_num++;
	}










	//Close the file if still open.
	if(fp) {
		fclose(fp);
	}
   	return(0);
}

int main(int argc, char** argv)
{

	if(argc == 3)
	{
		if(!(!strcmp(argv[1], verbose) || !strcmp(argv[1], suppress)))
		{
			printf("Usage:\n\t./styleguide <opcode> <filepath>\n\n\t-s suppresses all warnings (thing that only *might* be errors)\n\t-v prints all errors and warnings\n\n");
			return 0;
		}

		filestring = argv[2];

		if(strcmp(argv[1], suppress) == 0)
		{
			mode = SUPPRESS;
		}
		else if(strcmp(argv[1], verbose) == 0)
		{
			mode = VERBOSE;
		}
		else
		{
			mode = VERBOSE;
		}

	}
	else
	{
		printf("Usage:\n\t./styleguide <opcode> <filepath>\n\n\t-s suppresses all warnings (thing that only *might* be errors)\n\t-v prints all errors and warnings\n\n");
		return 0;
	}

	search(filestring);

	return 0;
}
