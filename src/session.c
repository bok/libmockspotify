#include <stdlib.h>
#include <string.h>
#include "libmockspotify.h"

/*** MockSpotify API ***/

event_type eventq[16];
int events = 0;

/*** Spotify API ***/

sp_playlistcontainer *
sp_session_playlistcontainer(sp_session *session)
{
    return mocksp_playlistcontainer_create();
}

const char * sp_build_id(void)
{
  return "9.1.32.g8edbd53c Release (Core: 0.5.3.425.g18537e22)";
}

sp_error
sp_session_create(const sp_session_config * config, sp_session ** sess)
{
    if (memcmp(config->application_key, "appkey_good", config->application_key_size))
        return SP_ERROR_BAD_APPLICATION_KEY;

    *sess = ALLOC(sp_session);
    sp_session *session = *sess;

    session->config.cache_location    = malloc(strlen(config->cache_location) + 1);
    session->config.settings_location = malloc(strlen(config->settings_location) + 1);
    session->config.application_key   = malloc(config->application_key_size);
    session->config.user_agent        = malloc(strlen(config->user_agent) + 1);
    session->config.callbacks         = ALLOC(sp_session_callbacks);
    session->config.userdata          = config->userdata;

    session->config.api_version       = config->api_version;
    strcpy((char *)session->config.cache_location, config->cache_location);
    strcpy((char *)session->config.settings_location, config->settings_location);
    memcpy((char *)session->config.application_key, config->application_key, config->application_key_size);
    strcpy((char *)session->config.user_agent, config->user_agent);
    memcpy((char *)session->config.callbacks, config->callbacks, sizeof(sp_session_callbacks));
    session->config.userdata          = config->userdata;

    // TODO: v0.0.8 (and earlier) directly call `notify_main_thread` callback here, before returning
    if (config->callbacks->notify_main_thread)
      config->callbacks->notify_main_thread(NULL);

    return SP_ERROR_OK;
}

void
sp_session_process_events(sp_session *session, int *next_timeout)
{
    if (events > 0) {
        events--;
        event_type next = eventq[events];

        if (next == MOCK_LOGGED_IN) {
            session->config.callbacks->logged_in(session, SP_ERROR_OK);
        }
    }
    *next_timeout = 1;
}

sp_playlist *
sp_session_starred_create(sp_session *session)
{
    return mocksp_playlist_create("Starred");
}

sp_error
sp_session_player_load(sp_session *session, sp_track *track)
{
    return 0;
}

void
sp_session_player_seek(sp_session *session, int offset)
{
}

void
sp_session_player_play(sp_session *session, bool b)
{
}

void
sp_session_player_unload(sp_session *session)
{
}

sp_connectionstate
sp_session_connectionstate(sp_session *session)
{
    return session->connectionstate;
}

void *
sp_session_userdata(sp_session *session)
{
    return session->config.userdata;
}

void
sp_session_login(sp_session *session, const char *username, const char *password, bool remember_me)
{
    session->connectionstate = SP_CONNECTION_STATE_LOGGED_IN;
    if (remember_me)
    {
      my_strncpy(session->username, username, 1024);
    }
    eventq[events++] = MOCK_LOGGED_IN;
}

void
sp_session_logout(sp_session *session)
{
  session->connectionstate = SP_CONNECTION_STATE_LOGGED_OUT;
}

sp_user *
sp_session_user(sp_session *session)
{
  return mocksp_user_create(session->username, "", "", "", SP_RELATION_TYPE_NONE, 1);
}