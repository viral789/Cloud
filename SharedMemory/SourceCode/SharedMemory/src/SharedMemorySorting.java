import java.io.*;
import java.util.*;

import sun.reflect.ReflectionFactory.GetReflectionFactoryAction;


public class SharedMemorySorting implements Runnable{

	/**
	 * @param args
	 */
	
	public static final int TOTAL_CHUNKS = 10;
	public static List<String> 	files = new ArrayList<String>(); ;
	public static long fSize = 0;
	int w=0;
	String[] no_of_records;
	public static long totalTime = 0;
	public SharedMemorySorting(){
		
	}
	public SharedMemorySorting(String[] no_of_records){
		this.no_of_records = no_of_records;
	}
	public void run(){
		SharedMemorySorting sha = new SharedMemorySorting();	
		sha.mergeSort(no_of_records);//calling run method of each thread with unique records set
	}
	/*main method*/
	public static void main(String[] args) {
		// TODO Auto-generated method stub
	
		SharedMemorySorting sha = new SharedMemorySorting();
	
		
		fSize = new File(args[0]).length();//length of input file
		sha.createFilesForSorting(4,args[0]);//invoke function to divide and sort input file
		
		sha.combineSortedFiles(files,fSize);//merge all sorted chunks
		
		System.out.println("The time taken is "+totalTime+" ms");
		
		/*delete the temp file */
		for(int i=0;i<files.size();i++){
			new File(files.get(i)).deleteOnExit();
		}

	}

	/*Function for merging all sorted chunks files*/
	private void combineSortedFiles(List<String> recordFiles, long fileSize) {
		// TODO Auto-generated method stub
		int numFiles = recordFiles.size();
		//System.out.println("time:"+w);
		BufferedReader br[] = new BufferedReader[numFiles];
		String brInfo =null;
		
		try{
			String[] data = new String[files.size()];
			for(int i=0;i<files.size();i++){ 
				br[i] = new BufferedReader(new FileReader("/mnt/raid/chunk"+(i+1)+".txt"));//bufferreader for all files
				data[i] = br[i].readLine();
			}
			
			BufferedWriter bw = new BufferedWriter(new FileWriter(args[1],true));//bufferwriter for output file
			
			for(int i=0;i<fileSize/100;i++)
			{
				String start = data[0];
				int minNum = 0;
				
				for(int j=0; j<numFiles;j++)
				{
					if(data[j]!=null && start!=null && start.substring(0, 10).compareTo(data[j].substring(0, 10)) > 0)//comparing with min record
					{
						start = data[j];
						minNum = j;
					}
				}
				if(start != null)
					bw.write(start+"\n");//writing min record to output
				
				data[minNum] = br[minNum].readLine();
				
			}
			for(int j=0; j<numFiles;j++)
			{
				br[j].close();
			}
			bw.close();
			w++;
		}
		catch(Exception e)
		{
			e.printStackTrace();
		}
		
		
	}

	/*creating and sorting temp files*/
	private void createFilesForSorting(int numThreads,String fName) {
		// TODO Auto-generated method stub
		
		BufferedWriter bw = null;
		File divideFileObj = null;
		RandomAccessFile randomFileObj = null;
		int eachChunkSize = 0;
		int eachRecordSize =0,k=0;;
		
		
		try{	
			randomFileObj = new RandomAccessFile(fName, "rw");
			eachChunkSize = (int) fSize / TOTAL_CHUNKS;//Total Chunks
			eachRecordSize = eachChunkSize/100;//Record Size
			int readChunkSize = 0;
			int readChunkNumber = 0;
			//System.out.println("fileSize:"+fSize+" chunkSize:"+eachChunkSize);
			while(readChunkNumber < TOTAL_CHUNKS)
			{
				readChunkSize = 0;
				String[] records = new String[eachRecordSize];
				readChunkNumber++;
				k=0;
				bw = new BufferedWriter(new FileWriter("/mnt/raid/chunk"+readChunkNumber+".txt"));////bufferreader array for temp file
				
				files.add("chunk"+readChunkNumber+".txt");//add chunk file name to list
				
				while(k<(eachChunkSize/100) && readChunkSize < eachChunkSize)
				{
					records[k] = randomFileObj.readLine();//reading each record from file
					readChunkSize +=  records[k].length()+2;//adding 2 for line break
					k++;
				}
				
				
				long start = System.currentTimeMillis();//starting timer for sorting
				
				for (int j=0;j<numThreads;j++){
					SharedMemorySorting sha = new SharedMemorySorting(records);
					Thread t1 = new Thread(sha);//creating thread for each record array
					t1.start(); // starting thread
					t1.join();
				}
				
				totalTime+=System.currentTimeMillis()-start;//time for each sort and adding to total time

				for (int i=0;i<records.length;i++) {
					
					bw.write(records[i]+"\n");//write the sorted chunks to temporary files
				}
				bw.close();//close the buffered writer
				
			}
			
		}
		catch(Exception e)
		{
			System.out.println("error");
			e.printStackTrace();
		}
		finally{
			try {
				bw.close();//close the buffered writer
				randomFileObj.close(); //close the file handle
			} catch (Exception e) {
				e.printStackTrace();
			}
		}
	}
	/*merge sort*/
	private void mergeSort(String[] records) {
		// TODO Auto-generated method stub
		
		if(records.length >= 2)
		{
			String[] left = new String[records.length/2];
			//System.out.println("left Length:"+left.length);
			String[] right = new String[records.length - records.length/2];
			
			//System.out.println("right length:"+right.length);
			for(int i = 0; i < left.length; i++)
			{
				left[i] = records[i]; 
			}
			
			for(int i = 0; i < right.length; i++)
			{
				right[i] = records[i+records.length/2]; 
			}
			
			mergeSort(left);
			mergeSort(right);
			merge(records,left,right);
		}
		
	}
	/*merging*/
	private void merge(String[] records, String[] left, String[] right) {
		// TODO Auto-generated method stub
		int leftCount = 0;
		int rightCount = 0;
		for (int i = 0; i < records.length; i++) {
			if (rightCount >= right.length || (leftCount < left.length && left[leftCount].substring(0, 10).compareTo(right[rightCount].substring(0, 10)) < 0)) { //comapring with first 10 byte key
				records[i] = left[leftCount]; 
				leftCount++;
			} else {
				records[i] = right[rightCount];
				rightCount++;
			}
		}
		
	}

}
