import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.SocketException;


public class ServerUDP implements Runnable
{

	public static DatagramSocket serverSock = null;
	public static int threadID;
	
	public static void main(String[] args) throws SocketException, InterruptedException 
	{
		int []numOfThreads = {1,2,4,8};
		int threadLength = numOfThreads.length;
		serverSock = new DatagramSocket(5004);
		System.out.println("Server is started");
		
		for (int i = 0; i < threadLength; i++)
		{
			ServerUDP serverUDP = new ServerUDP();
			serverUDP.threadID = numOfThreads[i];
			new Thread(serverUDP).start();
			Thread.sleep(100);
		}
		
	}
	
	@Override
	public void run()
	{
		try
		{
			int blockSize = 65505;
			byte [] recvData = new byte[blockSize];
		
			DatagramPacket datagramPacket = new DatagramPacket(recvData, recvData.length);
			serverSock.receive(datagramPacket);
			recvData = datagramPacket.getData();
			
			datagramPacket = new DatagramPacket(recvData, recvData.length, datagramPacket.getAddress(), datagramPacket.getPort());
			serverSock.send(datagramPacket);
			
			System.out.println("Packet Sent from Server Side");
		}
		catch(Exception e)
		{
			e.printStackTrace();
		}
	}
}
