package com.fang.myapplication.player;


import android.content.Context;
import android.os.Handler;
import android.os.HandlerThread;
import android.os.Message;
import android.os.SystemClock;
import android.util.Log;
import android.view.Surface;

import com.fang.myapplication.model.NALPacket;
import com.fang.myapplication.model.PCMPacket;

public class AVPlayer implements MediaTimeProvider, Handler.Callback {

    private static final String TAG = "AVPlayer";

    private static final int MSG_DO_SOME_WORK = 2;

    private VideoPlayer mVideoPlayer;
    private AudioPlayer mAudioPlayer;
    private Handler mHandler;
    private HandlerThread mRenderThread;

    public AVPlayer(Context context) {
        mAudioPlayer = new AudioPlayer();
        mVideoPlayer = new VideoPlayer(context, this);
        mRenderThread = new HandlerThread("Render");
        mRenderThread.start();
        mHandler = new Handler(mRenderThread.getLooper(), this);
    }

    public void start() {
        mVideoPlayer.start();
        mAudioPlayer.start();
        mHandler.sendEmptyMessage(MSG_DO_SOME_WORK);
    }

    public void stop() {
        mHandler.removeCallbacks(null);
        mVideoPlayer.stop();
    }

    public void setSurface(Surface surface) {
        mVideoPlayer.setSurface(surface);
    }

    @Override
    public boolean handleMessage(Message msg) {
        switch (msg.what) {
            case MSG_DO_SOME_WORK:
                doSomeWork();
                break;
        }
        return false;
    }

    /* video */
    public void addPacker(NALPacket nalPacket) {
        mVideoPlayer.addPacker(nalPacket);
    }

    /* audio */
    public void addPacker(PCMPacket pcmPacket) {
        mAudioPlayer.addPacker(pcmPacket);
    }

    private void doSomeWork() {
        mVideoPlayer.doSomeWork();
        mAudioPlayer.doSomeWork();
        //Log.d("AVSYNC","audio play pts = " + mAudioPlayer.getPteTest());

        mHandler.sendEmptyMessageDelayed(MSG_DO_SOME_WORK, 5);
    }

    @Override
    public long getAudioUs() {
        return mAudioPlayer.getPositionUs();
    }

    @Override
    public void setStartTime(long time) {
        mAudioPlayer.setStartTime(time);
    }

    public void destroy() {
        stop();
        mRenderThread.quit();
    }

}
