
import org.apache.hadoop.io.Text;
import org.apache.hadoop.mapreduce.Reducer;


public class HadoopReducer extends Reducer<Text, Text, Text, Text> {

	/*Reduce method of Reducer*/
	 @Override
	    protected void reduce(Text key, Iterable<Text> values, Context context)  {
		 try {
	        for(Text val:values){
					context.write(key, val);//writing each key value pair to output
	        }
		 } 
		 catch (Exception e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
		} 
	 }
}
