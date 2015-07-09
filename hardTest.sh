cat testInstances.txt | while read line
do
	PARA=" ../../data/"$line	
	./solver-v2/src/xyz $PARA
done
#	PARAMETERS="--main::method TS --main::output_file outTest.txt --main::instance "$line
#	./xyz $PARAMETERS
	 
