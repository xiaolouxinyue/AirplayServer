package com.fang.myapplication.player;

public interface MediaTimeProvider {
    long getAudioUs();
    void setStartTime(long time);
}
