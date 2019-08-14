/*
 * MIT License
 *
 * Copyright (c) 2019 dsafa22, All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
package com.fang.myapplication.player;

import android.annotation.SuppressLint;
import android.annotation.TargetApi;
import android.media.AudioFormat;
import android.media.AudioManager;
import android.media.AudioTrack;
import android.os.SystemClock;
import android.util.Log;

import com.fang.myapplication.model.PCMPacket;
import com.google.android.exoplayer2.C;

import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.util.ArrayList;
import java.util.Collections;
import java.util.List;

public class AudioPlayer {

    private static final String TAG = "AudioPlayer";

    @SuppressLint("InlinedApi")
    private static final int WRITE_NON_BLOCKING = AudioTrack.WRITE_NON_BLOCKING;

    private AudioTrack mAudioTrack;
    private List<PCMPacket> mSyncList = Collections.synchronizedList(new ArrayList<>());
    private volatile boolean mIsStart = false;
    private final AudioTrackPositionTracker audioTrackPositionTracker;
    private long firstPts = 0;
    private ByteBuffer avSyncHeader;
    private int bytesUntilNextAvSync;

    public AudioPlayer() {
        int audioFormat = AudioFormat.ENCODING_PCM_16BIT;
        int sampleRate = 44100;
        int channel = AudioFormat.CHANNEL_OUT_STEREO;
        int bufferSize = AudioTrack.getMinBufferSize(sampleRate, channel, audioFormat);
        mAudioTrack = new AudioTrack(AudioManager.STREAM_MUSIC, sampleRate, channel, audioFormat,
                bufferSize, AudioTrack.MODE_STREAM);

        audioTrackPositionTracker = new AudioTrackPositionTracker(new com.fang.myapplication.player.AudioTrackPositionTracker.Listener() {
            @Override
            public void onPositionFramesMismatch(long audioTimestampPositionFrames, long audioTimestampSystemTimeUs, long systemTimeUs, long playbackPositionUs) {

            }

            @Override
            public void onSystemTimeUsMismatch(long audioTimestampPositionFrames, long audioTimestampSystemTimeUs, long systemTimeUs, long playbackPositionUs) {

            }

            @Override
            public void onInvalidLatency(long latencyUs) {

            }

            @Override
            public void onUnderrun(int bufferSize, long bufferSizeMs) {

            }
        });
        audioTrackPositionTracker.setAudioTrack(mAudioTrack, C.ENCODING_PCM_16BIT, 4, bufferSize);
    }

    public void addPacker(PCMPacket pcmPacket) {
        mSyncList.add(pcmPacket);
        if (firstPts == 0) {
            firstPts = pcmPacket.pts;
        }
    }

    public void doSomeWork() {
        if (!mSyncList.isEmpty()) {
            PCMPacket pcmPacket = mSyncList.remove(0);
            Log.d("AVSYNC","audio process pts = " + pcmPacket.pts);
            long time = System.currentTimeMillis();
            // 错误
//            for (short s : pcmPacket.data) {
//                byteBuf.putShort(s);
//            }
//            ByteBuffer byteBuf = ByteBuffer.allocate(1920);
//            byteBuf.put(short2byte(pcmPacket.data));
//            byteBuf.flip();
            //mAudioTrack.write(pcmPacket.data, 0, 960);
            mAudioTrack.write(short2byte(pcmPacket.data), 0, 1920);
            //int written = writeNonBlockingWithAvSyncV21(mAudioTrack, byteBuf, 1920, pcmPacket.pts);
//            if (written < 0) {
//                throw new RuntimeException("Audiotrack.write() failed.");
//            }
            Log.d("TEST","time = " + (System.currentTimeMillis() - time));
            //mIsStart = true;
        }
    }

    @TargetApi(21)
    private static int writeNonBlockingV21(AudioTrack audioTrack, ByteBuffer buffer, int size) {
        return audioTrack.write(buffer, size, WRITE_NON_BLOCKING);
    }

    @TargetApi(21)
    private int writeNonBlockingWithAvSyncV21(AudioTrack audioTrack, ByteBuffer buffer, int size,
                                              long presentationTimeUs) {
        // TODO: Uncomment this when [Internal ref: b/33627517] is clarified or fixed.
        // if (Util.SDK_INT >= 23) {
        //   // The underlying platform AudioTrack writes AV sync headers directly.
        //   return audioTrack.write(buffer, size, WRITE_NON_BLOCKING, presentationTimeUs * 1000);
        // }
        if (avSyncHeader == null) {
            avSyncHeader = ByteBuffer.allocate(16);
            avSyncHeader.order(ByteOrder.BIG_ENDIAN);
            avSyncHeader.putInt(0x55550001);
        }
        if (bytesUntilNextAvSync == 0) {
            avSyncHeader.putInt(4, size);
            avSyncHeader.putLong(8, presentationTimeUs * 1000);
            avSyncHeader.position(0);
            bytesUntilNextAvSync = size;
        }
        int avSyncHeaderBytesRemaining = avSyncHeader.remaining();
        if (avSyncHeaderBytesRemaining > 0) {
            int result = audioTrack.write(avSyncHeader, avSyncHeaderBytesRemaining, WRITE_NON_BLOCKING);
            if (result < 0) {
                bytesUntilNextAvSync = 0;
                return result;
            }
            if (result < avSyncHeaderBytesRemaining) {
                return 0;
            }
        }
        int result = writeNonBlockingV21(audioTrack, buffer, size);
        if (result < 0) {
            bytesUntilNextAvSync = 0;
            return result;
        }
        bytesUntilNextAvSync -= result;
        return result;
    }


    private byte[] short2byte(short[] sData) {
        int shortArrsize = sData.length;
        byte[] bytes = new byte[shortArrsize * 2];
        for (int i = 0; i < shortArrsize; i++) {
            bytes[i * 2] = (byte) (sData[i] & 0x00FF);
            bytes[(i * 2) + 1] = (byte) (sData[i] >> 8);
            sData[i] = 0;
        }
        return bytes;
    }

    public void start() {
        mIsStart = true;
        audioTrackPositionTracker.start();
        mAudioTrack.play();
    }

    public void stop() {
        mIsStart = false;
        if (mAudioTrack != null) {
            mAudioTrack.flush();
            mAudioTrack.stop();
            mAudioTrack.release();
            mAudioTrack = null;
        }
        firstPts = 0;
    }

    public long getPositionUs() {
        if (mIsStart && firstPts != 0) {
            Log.d("AVSYNC","use audio pts");
            return initUs + firstPts + audioTrackPositionTracker.getCurrentPositionUs(false);
        }
        return SystemClock.elapsedRealtime() * 1000;
    }

    long initUs = 0;
    public void setStartTime(long time) {
        initUs = time;
    }

    public boolean isStart() {
        return mIsStart;
    }
}
