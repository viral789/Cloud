import java.io.IOException;

import org.apache.hadoop.mapreduce.Mapper;
import org.apache.hadoop.mapreduce.Mapper.Context;
import org.apache.hadoop.io.LongWritable;
import org.apache.hadoop.io.Text;

public class HadoopMapper extends Mapper<LongWritable, Text, Text, Text>  {

	/*map method for mapper class*/
	@Override
	protected void map(LongWritable key, Text value, Context context) throws IOException, InterruptedException {
		// TODO Auto-generated method stub
		
		Text k = new Text();//key object
		k.set(value.toString().substring(0, 10));//setting first 10 byte as key
		Text v = new Text();//value object
		v.set(value.toString().substring(10,value.toString().length()));//setting rest as value 
		context.write(k, v);//writing key-value pair
	}
	
}
