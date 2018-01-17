import java.io.*;
import java.net.*;


public class ClientTCP implements Runnable 
{
	static Socket clientSock;
	private int threadID;

	public static void main(String args[]) throws UnknownHostException, IOException, Exception 
	{
		int []numOfThreads = {1,2,4,8};
		int threadLength = numOfThreads.length;
		clientSock = new Socket("127.0.0.1", 5004);	// IP address and port of the machine. Must be Either Localhost(127.0.0.1) or private IP address

		System.out.println("Connected to Server Successfully");

		//Initiate the clients as per no. of Threads inputed
		for (int i = 0; i < threadLength; i++)
		{
			ClientTCP clientTCP = new ClientTCP();
			clientTCP.threadID = numOfThreads[i];
			new Thread(clientTCP).start();
			Thread.sleep(1000);
		}
	}

	public void run() 
	{
		try 
		{
			int blockSize = 65505;
				//Create output stream to interact with sockets
				DataOutputStream dataOutputStream = new DataOutputStream(clientSock.getOutputStream());

				File inputFile = new File("Network.txt");
				long fileSize = inputFile.length();
				FileInputStream fileInputStream = new FileInputStream(inputFile);

				byte[] bufContent = new byte[blockSize];
				int numRead = 0;
				int totalBytes = 0;
				
				double startTime = System.nanoTime();

				//read the data from the file
				while ((numRead = fileInputStream.read(bufContent)) != -1) 
				{
					dataOutputStream.write(bufContent, 0, numRead);
					totalBytes+=numRead;
					if (totalBytes == fileSize)
						break;
				}

				//make a file and read the data back into it
				DataInputStream dataInputStream = new DataInputStream(clientSock.getInputStream());
				File outputFile = new File("ClientTCP.txt");
				FileOutputStream fileOutputStream = new FileOutputStream(outputFile);
				numRead = 0;
				totalBytes = 0;
				bufContent = new byte[blockSize];

			
				while ((numRead = dataInputStream.read(bufContent)) !=-1) 
				{
					fileOutputStream.write(bufContent, 0, numRead);
					totalBytes+=numRead;
					if (totalBytes == fileSize)
						break;
				}

				double endTime = System.nanoTime();
				double totalTime = (endTime - startTime)/1000000;
				double throughput = (2*(blockSize/(1024.0*1024.0)))/(((totalTime)/1000));
				double latency = (totalTime)/2;

				System.out.println("****Executing for TCP for "+blockSize+"Bytes****");
				System.out.println("Total Time"+totalTime);
				System.out.println("Throughput for "+threadID+"threads is: "+throughput);
				System.out.println("Latency for "+threadID+"threads is: "+latency);
		}			
		catch (Exception e) 
		{
			e.printStackTrace();
		}
	}

}
