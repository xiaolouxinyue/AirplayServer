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

#include "raop_server.h"
#include "log.h"
#include "dnssd.h"
#include "logger.h"
#include <iostream>
#include <cmath>

static void *
audio_init(void *cls)
{
    audio_session_t *session;
    session = (audio_session_t *)calloc(1, sizeof(audio_session_t));
    session->volume = 1.0f;
    return session;
}
static void
audio_destroy(void *cls, void *opaque)
{
    auto *session = (audio_session_t *)opaque;
    free(session);
}
// 处理音量变化
static void
audio_set_volume(void *cls, void *opaque, float volume)
{
    auto *session = (audio_session_t *)opaque;
    session->volume = pow(10.0, 0.05*volume);
}

static void
log_callback(void *cls, int level, const char *msg) {
    switch (level) {
        case LOGGER_DEBUG: {
            LOGD("%s", msg);
            break;
        }
        case LOGGER_WARNING: {
            LOGW("%s", msg);
            break;
        }
        case LOGGER_INFO: {
            LOGI("%s", msg);
            break;
        }
        case LOGGER_ERR: {
            LOGE("%s", msg);
            break;
        }
        default:break;
    }
}

RaopServer::RaopServer(const std::string& device_name, char* hw_addr, int& hw_addr_len)
    :device_name_(device_name),
    hw_addr_(hw_addr),
    hw_addr_len_(hw_addr_len){}

RaopServer::~RaopServer() {

}

int RaopServer::StartServer(void *cls, AudioDataCallback audio_data_callback, VideoDataCallback video_data_callback) {
    raop_callbacks_t raop_cbs;
    memset(&raop_cbs, 0, sizeof(raop_cbs));
    raop_cbs.cls = cls;
    raop_cbs.audio_init = audio_init;
    raop_cbs.audio_process = audio_data_callback;
    raop_cbs.audio_destroy = audio_destroy;
    raop_cbs.audio_set_volume = audio_set_volume;
    raop_cbs.video_process = video_data_callback;
    raop_ = raop_init(10, &raop_cbs);
    if (raop_ == NULL) {
        LOGE("raop = NULL");
        return -1;
    }
    else {
        LOGD("raop init success");
    }
    raop_set_log_callback(raop_, log_callback, NULL);
    raop_set_log_level(raop_, RAOP_LOG_DEBUG);
    unsigned short port = 0;
    raop_start(raop_, &port);
    raop_set_port(raop_, port);
    int error;
    dnssd_ = dnssd_init(device_name_.c_str(), hw_addr_, hw_addr_len_, &error);
    if (dnssd_ == NULL) {
        LOGD("dnssd init error = %d", error);
        raop_destroy(raop_);
        return -1;
    }
    dnssd_register_raop(dnssd_, port);
    dnssd_register_airplay(dnssd_, port + 1);
    LOGD("raop port = %d, airplay port = %d", port, port + 1);
    return 0;
}

void RaopServer::StopServer() {
    raop_destroy(raop_);
    dnssd_unregister_raop(dnssd_);
    dnssd_unregister_airplay(dnssd_);
    dnssd_destroy(dnssd_);
}

void *RaopServer::GetRaopCls() {
    return raop_get_callback_cls(raop_);
}

int RaopServer::GetRaopPort() {
    return raop_get_port(raop_);
}


