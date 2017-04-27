# styleguide
A CMPS12B styleguide checker.
This is in early development and definitely doesn't exhaustively check for every error, but there is no harm in running this.

## Usage

The expected input follows the form `./styleguide <opcode> <filepath>`

`-v` : verbose
  reports on all errors and warnings. Warnings *may* not be errors and are prone to give false positives, but hey, better safe than sorry, right?
  
  `-s` : suppress warnings
  reports on findings which are likely to be errors, but still may give false positives
  
`filepath`  : accepts all filetypes but will give useless responses for non-ascii files

## Installation

You can clone this repo with:

`git clone https://github.com/DylanCirimelli-Low/styleguide`

Otherwise, you can download the `.zip` directly.

## Example
Consider the following code
```
#include <stdint.h>

int main(void){
	
	uint8_t num = 6;
	char array_of_numbers[5] = {1,2,3,4,5};
	
	if(1 == 1){//always evaluates to TRUE
	printf("Hello, world!");
	}
	return 1;
}
```
Ran with `-v` the output will be

![example with verbose](https://cloud.githubusercontent.com/assets/20120525/25462841/a508f2c8-2aa6-11e7-8eb3-a38f9d364dcf.PNG)


## Known errors

This will still parse whatever is inside strings and inline comments as if they were code; single-line comments are okay though. This was puked up in an hour or two so it isn't anything great, but it is doesn't cost anything to run. If I have time, i'll finish after midterms.
