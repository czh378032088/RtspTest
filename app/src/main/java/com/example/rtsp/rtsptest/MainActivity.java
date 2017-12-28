package com.example.rtsp.rtsptest;

import android.content.Intent;
import android.media.MediaPlayer;
import android.net.Uri;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.text.TextUtils;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.TextView;
import android.widget.VideoView;
import android.widget.MediaController;
import android.util.Log;

import org.xutils.view.annotation.ViewInject;

import java.net.Socket;
import java.util.List;


public class MainActivity extends AppCompatActivity implements View.OnClickListener{

    // Used to load the 'native-lib' library on application startup.
    //@ViewInject(R.id.btn_start_play)
    private Button mBtnStartPlay;
    //@ViewInject(R.id.edit_net_address)
    private EditText mEditNetAddress;
    private P2PCtrl p2pCtrl;
/*
    static {
        System.loadLibrary("p2pnode-lib");
    }*/

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main_new);
        mBtnStartPlay = findViewById(R.id.btn_start_play);
        mEditNetAddress = findViewById(R.id.edit_net_address);
        p2pCtrl = new P2PCtrl();
        p2pCtrl.p2pstart("120.77.147.40","phoneplayer","GM8136cammera");
        mBtnStartPlay.setOnClickListener(this);

    }

    @Override
    public void onClick(View v) {
        if(v == mBtnStartPlay){
            String netAddress = mEditNetAddress.getText().toString().trim();

            if(!TextUtils.isEmpty(netAddress)){
                while(!p2pCtrl.p2pisconnected());
                Intent intent = new Intent(this, JavaActivity.class);
                intent.putExtra("extra_url", netAddress);
                startActivity(intent);
            }
        }
    }
}
