# styleguide
A CMPS12B styleguide checker.
This is in early development and definitely doesn't exhaustively check for every error, but there is no harm in running this.

## Usage
The expected input follows the form `./styleguide <opcode> <filepath>`

`-v` : verbose
  reports on all error and warnings. Warning *may* not be errors and are prone to give false positives, but hey, better safe than sorry, right?
  
  `-s` : suppress warnings
  reports on finding which are likely to be errors, but still may give false positives
  
`filepath`  : accepts all filetypes but will give useless responses for non-ascii files

## Installation

You can clone this repo with:

`git clone https://github.com/DylanCirimelli-Low/styleguide`

Otherwise, you can download the `.zip` directly.

##Example
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

