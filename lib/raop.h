#ifndef RAOP_H
#define RAOP_H

#include "stream.h"

#ifdef __cplusplus
extern "C" {
#endif


/* Define syslog style log levels */
#define RAOP_LOG_EMERG       0       /* system is unusable */
#define RAOP_LOG_ALERT       1       /* action must be taken immediately */
#define RAOP_LOG_CRIT        2       /* critical conditions */
#define RAOP_LOG_ERR         3       /* error conditions */
#define RAOP_LOG_WARNING     4       /* warning conditions */
#define RAOP_LOG_NOTICE      5       /* normal but significant condition */
#define RAOP_LOG_INFO        6       /* informational */
#define RAOP_LOG_DEBUG       7       /* debug-level messages */


typedef struct raop_s raop_t;

typedef void (*raop_log_callback_t)(void *cls, int level, const char *msg);

struct raop_callbacks_s {
	void* cls;
	/* pcm数据回调 */
	void* (*audio_init)(void *cls);
	void  (*audio_process)(void *cls, void *session, pcm_data_struct *data);
	void  (*audio_destroy)(void *cls, void *session);
	/* h264数据回调 */
    void  (*video_process)(void* cls, h264_decode_struct* data);
    void  (*video_destroy)(void* cls);
	/* Optional but recommended callback functions */
	void  (*audio_flush)(void *cls, void *session);
	void  (*audio_set_volume)(void *cls, void *session, float volume);
	void  (*audio_set_metadata)(void *cls, void *session, const void *buffer, int buflen);
	void  (*audio_set_coverart)(void *cls, void *session, const void *buffer, int buflen);
	void  (*audio_remote_control_id)(void *cls, const char *dacp_id, const char *active_remote_header);
	void  (*audio_set_progress)(void *cls, void *session, unsigned int start, unsigned int curr, unsigned int end);
};
typedef struct raop_callbacks_s raop_callbacks_t;

raop_t *raop_init(int max_clients, raop_callbacks_t *callbacks);

void raop_set_log_level(raop_t *raop, int level);
void raop_set_log_callback(raop_t *raop, raop_log_callback_t callback, void *cls);
void raop_set_port(raop_t *raop, unsigned short port);
unsigned short raop_get_port(raop_t *raop);
void *raop_get_callback_cls(raop_t *raop);
void raop_set_hw_addr(raop_t *raop, const char* hw_addr, int hw_addr_len);
int raop_start(raop_t *raop, unsigned short *port);
int raop_is_running(raop_t *raop);
void raop_stop(raop_t *raop);
void raop_reset_pairing_session(raop_t* raop);
void raop_destroy(raop_t *raop);

#ifdef __cplusplus
}
#endif
#endif
