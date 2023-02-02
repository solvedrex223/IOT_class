import java.io.DataInputStream;
import java.net.ServerSocket;
import java.net.Socket;

public class Server {

    Socket socket;
    ServerSocket server;
    DataInputStream in;

    Server(){
        try {
            this.server = new ServerSocket(5000);
            System.out.println("Server created");
            socket = server.accept();
            in = new DataInputStream(socket.getInputStream());
            String line = "";
            while (!line.equals("close")) {
                line = in.readUTF();
                System.out.println(line);
            }
        } 

        catch (Exception e) {
            System.err.println(e.getMessage());
        }
    }
    public static void main(String[] args) {
        new Server();
    } 
}