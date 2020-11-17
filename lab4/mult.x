/*combine the arguments to be passed to the server side in a structure*/

struct numbers{
	double a;
	double b;
};

program MULT_PROG{
	version MULT_VERS{
		double multiply(numbers)=1;
	}=1;
}=0x4562877;
