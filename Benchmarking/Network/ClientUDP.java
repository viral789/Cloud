
import java.io.*;
import java.net.*;

public class ClientUDP implements Runnable
{
	public static DatagramSocket clientSock;
	public static int threadID;
	
	public static void main(String[] args) throws SocketException, UnknownHostException, InterruptedException 
	{
		// TODO Auto-generated method stub
		int []numOfThreads = {1,2,4,8};
		int threadLength = numOfThreads.length;
		clientSock = new DatagramSocket();
		
		//do for num. of threads time
		for (int i = 0; i < threadLength; i++)
		{
			ClientUDP clientUDP = new ClientUDP();
			clientUDP.threadID = numOfThreads[i];
			new Thread(clientUDP).start();
			Thread.sleep(100);
		}
	}

	@Override
	public void run()
	{
		try
		{
			int blockSize = 65505;
			//get the INET Address for sending packet to server
			InetAddress iNAddress = InetAddress.getByName("127.0.0.1");	//Change IP Required here !!!
			File file = new File("Network.txt");
		
			byte [] sendData = new byte[blockSize];
			byte [] recvData = new byte[blockSize];
			
			//read the data from a file to send to server
			FileInputStream fileInputStream = new FileInputStream(file);
			fileInputStream.read(sendData);
		
			double startTime = System.nanoTime();
		
			//create a datagram packet to be sent onto the Server side
			DatagramPacket datagramPacket = new DatagramPacket(sendData, sendData.length, iNAddress, 5004);
			clientSock.send(datagramPacket);
	
			//receive the data back into a packet on client sidde
			datagramPacket = new DatagramPacket(recvData, recvData.length);	
			clientSock.receive(datagramPacket);
			
		
			double endTime = System.nanoTime();
			double totalTime = (endTime-startTime)/(1000000000);
		
			//calculate the throughput and latency
			double throughput = 2*((blockSize/totalTime)/(1024*1024));
			double latency = (totalTime*1000)/2;
		
			System.out.println("****Executing for UDP for ****"+blockSize+"Bytes");
			System.out.println("Total Time"+totalTime);
			System.out.println("Throughput for "+threadID+"threads is: "+throughput);
			System.out.println("Latency for "+threadID+"threads is: "+latency);
		}
		catch(Exception e)
		{
			e.printStackTrace();
		}
	}

}
