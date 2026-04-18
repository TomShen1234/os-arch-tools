long int i;

long int array1[4] = {2, 3, 5, 8}, array2[4] = {5, 6, 11, 17}, array3[4];

int main()
{
	for ( i = 0 ; i < 4 ; i++ )
		array3[i] = array1[i] + array2[i];
	return 0;
}
