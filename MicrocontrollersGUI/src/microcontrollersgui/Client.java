/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package microcontrollersgui;

import java.io.DataOutputStream;
import java.io.IOException;
import java.net.Socket;
import java.util.logging.Level;
import java.util.logging.Logger;

/**
 *
 * @author Sachi
 */
public class Client {
    
//    private final String forward = "f\n\r";
//    private final String backward = "b";
//    private final String stop = "s";
//    private final String left = "l";
//    private final String right = "r";
//    private String lastMessage;
//    private boolean status;
    
    private Socket clientSocket;
    private DataOutputStream outToServer;
//    private BufferedWriter writer;
    private final String ip;
    private final int port;
    
    public Client(String ipaddress, int portaddress) {
//        lastMessage = "released";
//        status = true;
        this.ip = ipaddress;
        this.port = portaddress;
        connectToServer();
    }
    
    private boolean connectToServer() {
        
        try {
            clientSocket = new Socket(ip, port);
            outToServer = new DataOutputStream(clientSocket.getOutputStream());
            return true;
        } catch (IOException ex) {
            ex.printStackTrace();
            System.out.println("Couldn't connect to server");
            try {
                clientSocket.close();
            } catch (Exception e) {
            }
            return false;
        }
    }
    
    private boolean send(String msg) {
        try {
//            outToServer.writeBytes(msg+System.getProperty("line.separator"));
            outToServer.writeUTF(msg);
            return true;
        } catch (IOException ex) {
            Logger.getLogger(Client.class.getName()).log(Level.SEVERE, null, ex);
            return false;
        }
    }
    
    public void move(char input) {
        String msg;
        switch (input) {
            case 'w':
            case 'W':
               send("f\n\r");
//                msg = "move forward";
                break;
            case 's':
            case 'S':
                send("b");
//                msg = "move backward";
                break;
            case 'a':
            case 'A':
                send("l");
//                msg = "turn left";
                break;
            case 'd':
            case 'D':
                 send( "r");
//                msg = "turn right";
                break;
            default:
//                msg = "incorrect key";
                break;
        }
//        if (!status) {
//            msg = "something went wrong";
////            return msg;
//        }
//        if (!lastMessage.equals("released")) {
////            return ("null");
//        } else {
//            lastMessage = msg;
////            return msg;
//        }
    }
    
    public void stop() {
        boolean stat = send("s");
//        lastMessage = "released";
    }
    
    public void closeConnection() {
        try {
            clientSocket.close();
        } catch (IOException ex) {
            
        }
    }
    
}
