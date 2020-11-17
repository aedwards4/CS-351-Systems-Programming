/*combine the arguments to be passed to the server side in a structure*/

struct numbers{
	double a;
	double b;
};

program DIV_PROG{
	version DIV_VERS{
		double divide(numbers)=1;
	}=1;
}=0x4562877;
