import java.io.*;
import java.net.*;

public class ServerTCP implements Runnable 
{
	private static ServerSocket serverSocket;
	private Socket clientSock;

	public static void main(String[] args) 
	{
		try 
		{
			int []numOfThreads = {1,2,4,8};
			System.out.println("Server Started");
			
			//creating the serverSocket on port number
			serverSocket = new ServerSocket(5004);
			
			int threadLength = numOfThreads.length;

			Socket clientConn = serverSocket.accept();
			System.out.println("Client Connected");

			//create Thread for accepted connection
			for (int i = 0; i < threadLength; i++)
			{
				ServerTCP serverTCP = new ServerTCP();
				serverTCP.clientSock=clientConn;
				new Thread(serverTCP).start();
				Thread.sleep(1000);
			}
		} 
		catch (Exception e) 
		{
			e.printStackTrace();
		}
	}

	@Override
	public void run() 
	{

		try
		{			
			int blockSize = 65505;

				DataInputStream dataInputStream = new DataInputStream(clientSock.getInputStream());
				DataOutputStream dataOutputStream = new DataOutputStream(clientSock.getOutputStream());

				File inputFile = new File("ServerTCP.txt");
				long fileSize = inputFile.length();
				FileOutputStream fileOutputStream = new FileOutputStream(inputFile);
				FileInputStream fileInputStream = new FileInputStream(inputFile);

				byte[] buffContent = new byte[blockSize];
				int numRead = 0, totalReadingBytes=0;

				while ((numRead = dataInputStream.read(buffContent))!=-1) 
				{
					fileOutputStream.write(buffContent, 0, numRead);
					totalReadingBytes+=numRead;
					if (totalReadingBytes == fileSize)
						break;
				}

				numRead = 0;
				int totalWritingBytes = 0;
			
				while((numRead = fileInputStream.read(buffContent))!=-1)
				{		
					dataOutputStream.write(buffContent,0,numRead);
					totalWritingBytes+=numRead;
					if (totalWritingBytes == fileSize)
						break;
				}
	
		} 
		catch (Exception e) 
		{
			e.printStackTrace();
		}
	}
}
