package com.example.rtsp.rtsptest;

/**
 * Created by czh on 17-11-3.
 */

public class P2PCtrl {
    static {
        System.loadLibrary("p2pclient-lib");
    }
    public native void p2pstart(String ip,String ownername,String remotename);
    public native boolean p2pisconnected();
}
