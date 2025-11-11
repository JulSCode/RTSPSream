/*****************************************************************************
 * vlc.h: VLC media player - C interface
 * Based on VLC 4.0.0
 *****************************************************************************/

#ifndef VLC_H
#define VLC_H

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdint.h>
#include <stddef.h>

/* Opaque structures */
typedef struct libvlc_instance_t libvlc_instance_t;
typedef struct libvlc_media_t libvlc_media_t;
typedef struct libvlc_media_player_t libvlc_media_player_t;
typedef struct libvlc_event_manager_t libvlc_event_manager_t;
typedef struct libvlc_event_t libvlc_event_t;

/* State enum */
typedef enum libvlc_state_t {
    libvlc_Idle,
    libvlc_Opening,
    libvlc_Buffering,
    libvlc_Playing,
    libvlc_Paused,
    libvlc_Stopped,
    libvlc_Ended,
    libvlc_Error
} libvlc_state_t;

/* Core functions */
libvlc_instance_t *libvlc_new(int argc, const char *const *argv);
void libvlc_release(libvlc_instance_t *p_instance);

/* Media functions */
libvlc_media_t *libvlc_media_new_path(libvlc_instance_t *p_instance,
                                      const char *path);
void libvlc_media_release(libvlc_media_t *p_md);

/* Media player functions */
libvlc_media_player_t *libvlc_media_list_player_new(libvlc_instance_t *p_libvlc);
void libvlc_media_list_player_release(libvlc_media_player_t *p_mlp);
void libvlc_media_player_set_media(libvlc_media_player_t *p_mi,
                                   libvlc_media_t *p_md);

/* Playback control */
int libvlc_media_player_play(libvlc_media_player_t *p_mi);
void libvlc_media_player_set_pause(libvlc_media_player_t *p_mi, int do_pause);
void libvlc_media_player_stop(libvlc_media_player_t *p_mi);

/* State query */
libvlc_state_t libvlc_media_player_get_state(libvlc_media_player_t *p_mi);

/* Time and position */
int64_t libvlc_media_player_get_length(libvlc_media_player_t *p_mi);
int64_t libvlc_media_player_get_time(libvlc_media_player_t *p_mi);
void libvlc_media_player_set_time(libvlc_media_player_t *p_mi, int64_t i_time);

/* Error handling */
const char *libvlc_errmsg(void);

#ifdef __cplusplus
}
#endif

#endif /* VLC_H */
