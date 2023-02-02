import java.io.BufferedReader;
import java.io.DataOutputStream;
import java.io.InputStreamReader;
import java.net.Socket;

public class Client {
    
    Socket socket;
    DataOutputStream output;
    BufferedReader input;
    
    Client(){
        try {
            this.socket = new Socket("localhost", 5000);
            output = new DataOutputStream(socket.getOutputStream());
            input = new BufferedReader(new InputStreamReader(System.in));
            String line = "";
            while (!line.equals("close")) {
                line = input.readLine();
                output.writeUTF(line);
            }
        } catch (Exception e) {
            System.err.println(e.getMessage());
        }
    }
    public static void main(String[] args) {
        new Client();
    }
}
