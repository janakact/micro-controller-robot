/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package microcontrollersgui;

/**
 *
 * @author Sachi
 */
import java.io.*;
import java.net.*;

public class ServerTrial extends Thread {

    int port;

    public ServerTrial(int p) {
        port = p;
    }

    public void run() {
        try {
            ServerSocket serverSocket = new ServerSocket(port);
            Socket connectionSocket = serverSocket.accept();
            while (true) {
                DataInputStream datain = new DataInputStream(connectionSocket.getInputStream());
                System.out.println("received : " + datain.readUTF());
            }
        } catch (IOException ex) {
            System.out.println("Couldn't Listen to the client");
        }
    }

//    public static void main(String args[]) {
//        ServerTrial sp = new ServerTrial(5000);
//        sp.start();
//    }
}
