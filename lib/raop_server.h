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

#ifndef AIRPLAYSERVER_RAOP_SERVER_H
#define AIRPLAYSERVER_RAOP_SERVER_H

#include <iostream>
#include "raop.h"
#include "dnssd.h"

//#if defined (_WIN32) && defined(DLL_EXPORT)
//# define RAOP_SERVER_API __declspec(dllexport)
//#else
//# define RAOP_SERVER_API
//#endif

typedef struct {
    float volume;
} audio_session_t;

typedef void (*AudioDataCallback)(void *cls, void *session, pcm_data_struct *data);
typedef void (*VideoDataCallback)(void *cls, h264_decode_struct *data);

class RaopServer {
private:
    raop_t* raop_;
    dnssd_t* dnssd_;
    std::string device_name_;
    char* hw_addr_;
    int hw_addr_len_;

public:
    RaopServer(const std::string& device_name, char* hw_addr, int& hw_addr_len);
    ~RaopServer();
    int StartServer(void* cls, AudioDataCallback audio_data_callback, VideoDataCallback video_data_callback);
    void StopServer();
    void* GetRaopCls();
    int GetRaopPort();
};

#endif //AIRPLAYSERVER_RAOP_SERVER_H
