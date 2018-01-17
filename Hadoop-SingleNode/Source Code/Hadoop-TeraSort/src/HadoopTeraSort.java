import java.io.IOException;

import org.apache.hadoop.conf.Configuration;
import org.apache.hadoop.fs.Path;
import org.apache.hadoop.mapreduce.lib.input.FileInputFormat;
import org.apache.hadoop.mapreduce.lib.input.TextInputFormat;
import org.apache.hadoop.mapreduce.lib.output.FileOutputFormat;
import org.apache.hadoop.mapreduce.lib.output.TextOutputFormat;
import org.apache.hadoop.mapreduce.Job;
import org.apache.hadoop.io.Text;

public class HadoopTeraSort {

	/**
	 * @param args
	 */
	public static void main(String[] args) {
		// TODO Auto-generated method stub

		Configuration conn = new Configuration();//Creating Configuration Object
		try {
			Job j = Job.getInstance(conn,"HadoopTeraSort");//Setting Instance for Job
			j.setJarByClass(HadoopTeraSort.class);//Class File for JAR
			
			FileInputFormat.addInputPath(j, new Path(args[0]));//add input path to inputformat
			FileOutputFormat.setOutputPath(j, new Path(args[1]));//add output path to outputformat
			j.setInputFormatClass(TextInputFormat.class);
		    j.setOutputFormatClass(TextOutputFormat.class);
		    j.setMapOutputKeyClass(Text.class);
		    j.setMapOutputValueClass(Text.class);
			j.setOutputKeyClass(Text.class);
			j.setOutputValueClass(Text.class);
			j.setMapperClass(HadoopMapper.class);//defining mapper class
			j.setCombinerClass(HadoopReducer.class);//defining combiner class
			j.setReducerClass(HadoopReducer.class);//defining reducer class
			
			System.exit(j.waitForCompletion(true) ? 0 : 1);
			
			
			
			
		} catch (Exception e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		
	}

}
