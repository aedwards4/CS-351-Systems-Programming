/*combine the arguments to be passed to the server side in a structure*/
struct numbers{
	double a;
	double b;
};

program SUB_PROG{
	version SUB_VERS{
		double subtract(numbers)=1;
	}=1;
}=0x4562877;
